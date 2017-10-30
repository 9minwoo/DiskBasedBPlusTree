
/*
사용할 자료형 정리
signed 8바이트 : int64_t
unsigned 8바이트 : off_t, size_t
signed 4바이트 : int
key-> 8바이트이지만 범위가작으므로 int64_t사용
 */

/*
참고사항
-fp미사용,fd사용(fp를썻을때 fseek으로 전체 오프셋을 표현할 수 없어서)
-struct단위로 파일에 저장해서 편리하게했음.
-fd는 open함수로 열면 계속 열려 있게하고
 나머지 페이지들은 필요할때마다 offset을통해 하나씩 열고 닫고 하도록함.
-결국, 페이지를 하나씩 메모리에 올리고, free하고 그런다는거임. 
 */
#ifndef __BPT_H__
#define __BPT_H__

#define PAGE_SIZE 4096

typedef long long int64_t;
typedef unsigned long long size_t;
typedef unsigned long long off_t;

//meat_t 구조체는 헤더페이지에서 필요한 3개만 받아온다.
//밑에 빈 부분은 빈부분이라 메모리낭비라서 안받아올릴거임.
typedef struct meta_t {
   off_t free_offset;
   off_t root_offset;
   size_t num_of_pages;
} meta_t;
//차수를 나타내는 order
extern int order;
//전역변수로 fd설정해서 계속사용할거임
extern int fd;
//헤더페이지의 구조체는 전역변수로 설정해서 계속쓸수있도록함.
extern meta_t meta;

//인터널 페이지에 들어갈 배열의 구조체
typedef struct index_t {
   int64_t key;
   off_t child; /* child's offset */
} index_t;


//리프 페이지에 들어갈 배열의 구조체
typedef struct record_t {
   int64_t key;
   char value[120];
} record_t;


//in_momory 코드에서는 맨왼쪽 offset을 배열0번으로 잡아줬다
//	offset[0], key[0], offset[1] 이런 구조였다.
//dise-based에서는 internal과 leaf의 형식을 통일하고
//ppt방식대로 하기위해서 맨왼쪽 offset을 next로 따로하나 만들어줬다.
//	next, key[0], off[0], key[1], off[1]이런식으로 만듬.

//인터넬 페이지의 구조체
typedef struct internal_node_t {
   off_t parent_offset;
   int is_leaf;
   int num_keys;
   char reserved[104];
   off_t next;

   index_t children[248];
} internal_node_t;


//리프 페이지의 구조체
typedef struct leaf_node_t {
   off_t parent_offset;
   int is_leaf;
   int num_keys;
   char reserved[104];
   off_t next;

   record_t children[31];
} leaf_node_t;

//프리페이지의 구조체
typedef struct free_page_t {
   off_t next_free_offset;
   char empty[4088];
} free_page_t;

int open_db(char * pathname);
int insert(int64_t key, char * value);
char * find(int64_t key);
int delete(int64_t key);

#endif /* __BPT_H__*/