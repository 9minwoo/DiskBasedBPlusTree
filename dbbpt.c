/*
알아둘 것들.
disk_based.c 는 수업시간에 제공된 in_memory.c의 구조를 최대한 똑같이 따라했다.
disk_based를 구현할때 추가한 것들.
   file에 저장할 page들을 struct구조로 만들어 struct단위로
   입,출력을 편하게 사용하도록 했다.
   fd를 extern으로 잡아서 open_db함수가 끝난후에도 fd를 다른함수에서
   사용하여 입,출력을 가능하도록 했다.
   meta_t meta를 extern으로 잡아서 헤더페이지의 내용을 항상
   메모리에서 가지고 있도록 했고, update_meta를 통해서 meta의 변경내용을
   항상 disk에 기록하도록 했다.
 */



#include "bpt.h";

#include <string.h>

int open_db(char * pathname) {
   if ((fd = open(pathname, O_RDWR)) > 0) {
      read(fd, &meta, 24);
   }
   else if ((fd = creat(pathname, 0777)) > 0) {
      meta.free_offset = 0;
      meta.root_offset = 0;
      meta.num_of_pages = 0;

      create_empty_page();
      update_meta();

   }
   else {
      return -1;
   }

   return 0;
}
