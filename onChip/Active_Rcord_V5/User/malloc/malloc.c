#include "malloc.h"

//内存池
__align(32) u8 membase[MEM_MAX_SIZE];

//内存管理表
u16 memmapbase[MEM_ALLOC_TABLE_SIZE];

//内存管理参数
const u32 memblsize = MEM_BLOCK_SIZE;
const u32 memtablsize = MEM_ALLOC_TABLE_SIZE;
const u32 memsize = MEM_MAX_SIZE;

//内存管理控制器
struct _m_malloc_dev malloc_dev = {
	my_mem_init,
	my_mem_perused,
	membase,
	memmapbase,
	0,
};
/*
//复制内存
*des:目的地址
*src:源地址
n :需要复制的内存长度
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
//设置内存
//s :需要设置内存的首地址
//u8 c:内存初始化成的数值
//u32count:需要初始化的内存字节数
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


/**内存初始化**/
void my_mem_init(void)
{
	my_memset(malloc_dev.memmap,0,memtablsize*2);
	my_memset(malloc_dev.membase,0,memsize);
	malloc_dev.memready = 1;  //内存初始化完成标志
}	


/*
 *	内存返回值是使用的内存百分比
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



//内存分配，返回内存没有使用块的首地址相对偏移大小
//size:要分配内存大小（字节）
static u32 my_mem_malloc(u32 size)
{
	signed long offset = 0;//for循环块数
	u32 nmemb = 0; //需要的内存块数
	u32 cmemb = 0; //连续空内存块数
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
//释放内存
//内存偏移地址offset
//返回值：0:成功  1：释放失败
*/
static u8 my_mem_free(u32 offset)
{
	int i;
	if(!malloc_dev.memready){
		malloc_dev.init();
		return 1;
	}
	if(offset<memsize){
		int index = offset / memblsize;  //在内存块号码
		int nmemb = malloc_dev.memmap[index]; //查看内存数量
		for(i = 0;i<nmemb;i++){
			malloc_dev.memmap[index+i] = 0;
		}return 0;
	}else return 2;
}


//外部调用free，内存首地址减去当前地址
void my_free(void *ptr)
{
	u32 offset;
	if(ptr == NULL) return;
	offset = (u32)ptr - (u32)malloc_dev.membase;
	my_mem_free(offset);
}

//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u32 size)  
{  
    u32 offset;   
		offset= my_mem_malloc(size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)malloc_dev.membase+offset);  
}  
