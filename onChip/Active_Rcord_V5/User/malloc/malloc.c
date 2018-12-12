#include "malloc.h"

//�ڴ��
__align(32) u8 membase[MEM_MAX_SIZE];

//�ڴ�����
u16 memmapbase[MEM_ALLOC_TABLE_SIZE];

//�ڴ�������
const u32 memblsize = MEM_BLOCK_SIZE;
const u32 memtablsize = MEM_ALLOC_TABLE_SIZE;
const u32 memsize = MEM_MAX_SIZE;

//�ڴ���������
struct _m_malloc_dev malloc_dev = {
	my_mem_init,
	my_mem_perused,
	membase,
	memmapbase,
	0,
};
/*
//�����ڴ�
*des:Ŀ�ĵ�ַ
*src:Դ��ַ
n :��Ҫ���Ƶ��ڴ泤��
*/
void* my_memcpy(void *des,void *src,u32 n)
{
	char *des_addr = des;
	char *src_addr = src;
	if(NULL == des || NULL == src || n <= 0 )
	{
		return 	NULL;
	}
	while(n--){
		*des_addr++ = *src_addr++ ;
	}
	return des;
}
/*
//�����ڴ�
//s :��Ҫ�����ڴ���׵�ַ
//u8 c:�ڴ��ʼ���ɵ���ֵ
//u32count:��Ҫ��ʼ�����ڴ��ֽ���
*/
void* my_memset(void *s, u8 c, u32 count)
{
	u8*xs = s;
	if(count<=0 || NULL == s){
		return NULL;
	}
	while(count--){
		*xs++ = c;
	}
	return s;
}


/**�ڴ��ʼ��**/
void my_mem_init(void)
{
	my_memset(malloc_dev.memmap,0,memtablsize*2);
	my_memset(malloc_dev.membase,0,memsize);
	malloc_dev.memready = 1;  //�ڴ��ʼ����ɱ�־
}	


/*
 *	�ڴ淵��ֵ��ʹ�õ��ڴ�ٷֱ�
 */
u8 my_mem_perused(void)
{
	u32 used = 0;
	u32 i ;
	for(i = 0;i<memtablsize;i++){
		if(malloc_dev.memmap[i]) used++;
	}
	return (used *100) / memtablsize;
}



//�ڴ���䣬�����ڴ�û��ʹ�ÿ���׵�ַ���ƫ�ƴ�С
//size:Ҫ�����ڴ��С���ֽڣ�
static u32 my_mem_malloc(u32 size)
{
	signed long offset = 0;//forѭ������
	u32 nmemb = 0; //��Ҫ���ڴ����
	u32 cmemb = 0; //�������ڴ����
	u32 i = 0;
	
	if(!malloc_dev.memready) {
			malloc_dev.init();
	}
	
	if( 0 == size ) return 0xffffffff;
	nmemb = size / memblsize;
	if( size % memblsize) nmemb++;
	
	for(offset = memtablsize-1;offset>=0;offset--){
		if(!malloc_dev.memmap[offset]) cmemb++;
		else cmemb = 0;
		if(cmemb == nmemb){
				for(i = 0;i<nmemb;i++)
				{
					malloc_dev.memmap[offset+i] = nmemb;
				}
				return (offset*memblsize);
		}
	}
	return 0xffffffff;
}

/*
//�ͷ��ڴ�
//�ڴ�ƫ�Ƶ�ַoffset
//����ֵ��0:�ɹ�  1���ͷ�ʧ��
*/
static u8 my_mem_free(u32 offset)
{
	int i;
	if(!malloc_dev.memready){
		malloc_dev.init();
		return 1;
	}
	if(offset<memsize){
		int index = offset / memblsize;  //���ڴ�����
		int nmemb = malloc_dev.memmap[index]; //�鿴�ڴ�����
		for(i = 0;i<nmemb;i++){
			malloc_dev.memmap[index+i] = 0;
		}return 0;
	}else return 2;
}


//�ⲿ����free���ڴ��׵�ַ��ȥ��ǰ��ַ
void my_free(void *ptr)
{
	u32 offset;
	if(ptr == NULL) return;
	offset = (u32)ptr - (u32)malloc_dev.membase;
	my_mem_free(offset);
}

//�����ڴ�(�ⲿ����)
//memx:�����ڴ��
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(u32 size)  
{  
    u32 offset;   
		offset= my_mem_malloc(size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)malloc_dev.membase+offset);  
}  
