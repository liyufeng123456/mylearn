#include<iostream>
using namespace std;
#include<stdlib.h>
#include"ngx_pool.h"

//创建ngx的内存池
void NgxMemPool::ngx_create_pool(size_t size)
{
	_pool = (ngx_pool_t*)malloc(size);
	if(_pool==NULL)
	{
		return;
	}
	_pool->d.last = (u_char *)_pool + sizeof(ngx_pool_t);  //last指向ngx_pool_t结构体之后数据取起始位置  
	_pool->d.end = (u_char *)_pool + size;  //end指向分配的整个size大小的内存的末尾
	_pool->d.next = NULL;
	_pool->d.failed = 0;
	_pool->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;  //最大不超过4095B
	size = size - sizeof(ngx_pool_t);
	_pool->current = _pool;
	_pool->large = NULL;

}
//销毁ngx内存池
void NgxMemPool::ngx_destroy_pool()
{
	ngx_pool_large_t* p;
	//释放大块内存
	for (p = _pool->large;p != NULL; p = p->next)
	{
		if (p->alloc != NULL)
		{
			free(p->alloc);
		}
	}

	//释放内存池中的内存块
	ngx_pool_t *q = _pool;
	ngx_pool_t *s = q->d.next;
	while (1)
	{
		free(q);
		q = NULL;
		if (s == NULL)
		{
			break;
		}
		q = s;
		s = s->d.next;
	}
}
//重置内存池
void NgxMemPool::ngx_reset_pool()
{
	ngx_pool_t        *p;
	ngx_pool_large_t  *l;

	for (l = _pool->large; l; l = l->next) {
		if (l->alloc)
		{
			free(l->alloc);
		}
	}

	for (p = _pool; p; p = p->d.next) {
		p->d.last = (u_char *)p + sizeof(ngx_pool_t);
		p->d.failed = 0;
	}

	_pool->current = _pool;
	_pool->large = NULL;
}
//开辟内存，对齐
void* NgxMemPool::ngx_palloc(size_t size)
{
	if (size <= _pool->max) {
		return ngx_palloc_small(_pool, size, 1);
	}
	return ngx_palloc_large(_pool, size);
}
void* NgxMemPool::ngx_palloc_small(ngx_pool_t *_pool, size_t size, unsigned int a)
{
	u_char      *m;
	ngx_pool_t  *p;

	p = _pool->current;

	do {
		m = p->d.last;

		if (a)
		{
			m = ngx_align_ptr(m,sizeof(unsigned int));
		}

		if ((size_t)(p->d.end - m) >= size) {
			p->d.last = m + size;

			return m;
		}

		p = p->d.next;

	} while (p);

	return ngx_palloc_block(_pool, size);
}
void* NgxMemPool::ngx_palloc_large(ngx_pool_t *_pool, size_t size)
{
	void              *p;
	ngx_uint_t         n;
	ngx_pool_large_t  *large;

	p = malloc(size);
	if (p == NULL) {
		return NULL;
	}

	n = 0;

	for (large = _pool->large; large; large = large->next) {
		if (large->alloc == NULL) {
			large->alloc = p;
			return p;
		}

		if (n++ > 3) {
			break;
		}
	}
	large = (ngx_pool_large_t*)ngx_palloc_small(_pool, sizeof(ngx_pool_large_t), 1);
	if (large == NULL) {
		free(p);
		return NULL;
	}

	large->alloc = p;
	large->next = _pool->large;
	_pool->large = large;

	return p;
}
//开辟内存，不对齐
void* NgxMemPool::ngx_pnalloc(size_t size)
{
	if (size <= _pool->max) {
		return ngx_palloc_small(_pool, size, 0);
	}
	return ngx_palloc_large(_pool, size);
}
void* NgxMemPool::ngx_palloc_block(ngx_pool_t *_pool, size_t size)
{
	u_char      *m;
	size_t       psize;
	ngx_pool_t  *p;
	ngx_pool_t  *q;

	psize = (size_t)(_pool->d.end - (u_char *)_pool);

	m = (u_char*)malloc(psize);
	if (m == NULL) {
		return NULL;
	}

	q = (ngx_pool_t *)m;

	q->d.end = m + psize;
	q->d.next = NULL;
	q->d.failed = 0;

	m += sizeof(ngx_pool_data_t);
	m = ngx_align_ptr(m, sizeof(unsigned int));
	q->d.last = m + size;

	for (p = _pool->current; p->d.next; p = p->d.next) {
		if (p->d.failed++ > 4) {
			_pool->current = p->d.next;
		}
	}

	p->d.next = q;

	return m;
}

//把内存归还给内存池
void NgxMemPool::ngx_pfree(void *p)
{
	ngx_pool_large_t  *l;

	for (l = _pool->large; l; l = l->next) {
		if (p == l->alloc)
		{
			free(l->alloc);
			l->alloc = NULL;

			return;
		}
	}
}
int main()
{
	NgxMemPool gnxpool;
	gnxpool.ngx_create_pool(2*1024);
	cout << gnxpool.ngx_palloc(15) << endl;
	cout << gnxpool.ngx_palloc(15) << endl;
	cout << gnxpool.ngx_pnalloc(15) << endl;
	cout << gnxpool.ngx_pnalloc(15) << endl;
	gnxpool.ngx_reset_pool();
	cout << gnxpool.ngx_palloc(15) << endl;
	void * p = gnxpool.ngx_palloc(2*1024);
	cout << p << endl;
	gnxpool.ngx_pfree(p);
	gnxpool.ngx_destroy_pool();
	return 0;
}