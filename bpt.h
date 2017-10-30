#ifndef __BPT_H__
#define __BPT_H__

// Uncomment the line below if you are compiling on Windows.
// #define WINDOWS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef WINDOWS
#define bool char
#define false 0
#define true 1
#endif

// Default order is 4.
#define DEFAULT_ORDER 4

// Minimum order is necessarily 3.  We set the maximum
// order arbitrarily.  You may change the maximum order.
#define MIN_ORDER 3
#define MAX_ORDER 20

// Constants for printing part or all of the GPL license.
#define LICENSE_FILE "LICENSE.txt"
#define LICENSE_WARRANTEE 0
#define LICENSE_WARRANTEE_START 592
#define LICENSE_WARRANTEE_END 624
#define LICENSE_CONDITIONS 1
#define LICENSE_CONDITIONS_START 70
#define LICENSE_CONDITIONS_END 625

// TYPES.

/* Type representing the record
 * to which a given key refers.
 * In a real B+ tree system, the
 * record would hold data (in a database)
 * or a file (in an operating system)
 * or some other information.
 * Users can rewrite this part of the code
 * to change the type and content
 * of the value field.
 */
typedef struct record {
    int value;
} record;

/* Type representing a node in the B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 *     리프, 인터널 둘다에 같은구조 쓸수 있다.
 * The heart of the node is the array
 * of keys and the array of corresponding
 * pointers.  
 *     노드의 핵심은 키와 거기에 상응하는 포인터의 배열이다. 
 *     -> 아,, 노드하나에 키, 포인터가 배열로 들어가 있구나
 * The relation between keys
 * and pointers differs between leaves and
 * internal nodes.  
 *     키, 포인터 의 관계는 리프냐 인터널이냐에 따라 다름.
 * In a leaf, the index
 * of each key equals the index of its corresponding
 * pointer, with a maximum of order - 1 key-pointer
 * pairs. 
 *     리프에서는 , 키의 인덱스가 거기에 상응하는 포인터의 인덱스와 같다
 *     최대로 , order -1 개의 키-포인터 페어가 있다.
 *     이게무슨말이냐면, 인터널노드에서는 키,포인터쌍이 배열인덱스로는 1씩차이가난다
 *     왜냐하면, 포인터는 맨왼쪽에 하나 더있으니까!
 *     그런데 리프에서는 키,포인터쌍이 배열인덱스로도 똑같다 
 *     왜냐하면, 키는 키값을저장하고 그 키값이가지는 레코드를 포인터가 가리키기때문.     
 * The last pointer points to the
 * leaf to the right (or NULL in the case
 * of the rightmost leaf).
 *     리프노드의 마지막포인터는 오른쪽  leaf를 가리킨다.(없으면 NULL)
 * In an internal node, the first pointer
 * refers to lower nodes with keys less than
 * the smallest key in the keys array.  
 *     인터널 노드에서, 첫번째 포인터는 키배열의 smallest값보다
 *     작은 값들을 가지고있는 lower노드를 가리킨다.
 * Then,
 * with indices i starting at 0, the pointer
 * at i + 1 points to the subtree with keys
 * greater than or equal to the key in this
 * node at index i.
 *     그러고나서 i를 0부터 1씩 증가시키면서 포인터는 해당노드의 인덱스 i
 *     보다 같거나 큰 subtree를 가리킨다. 
 * The num_keys field is used to keep
 * track of the number of valid keys.
 *     num_keys 필드는 유효한 키들의 숫자를 저장한다.
 * In an internal node, the number of valid
 * pointers is always num_keys + 1.
 *     인터널노드에서, 유효한 포인터의 숫자는 항상 num_keys +1 이다.
 * In a leaf, the number of valid pointers
 * to data is always num_keys.
 *     리프에서, 데이터를 가리키는 유효한 포인터의 숫자는 항상 num_keys다
 * The last leaf pointer points to the next leaf.
 *     맨 마지막 리프포인터는 다음리프를 가리킨다.    
 */

//하위 노드를 가리키는pointer, 인덱스값인 key 둘다 각각의 배열에 저장한다~!
typedef struct node {
    void ** pointers;
    //**인거는 포인터들의 배열이기때문인거고
    //void인거는 leaf에서 레코드를 가리켜야되는데 그때 레코드 자료형을 몰라서?
    int * keys;
    struct node * parent;
    bool is_leaf;
    int num_keys;
    struct node * next; // Used for queue.
} node;

// GLOBALS.

/* The order determines the maximum and minimum
 * number of entries (keys and pointers) in any
 * node.  Every node has at most order - 1 keys and
 * at least (roughly speaking) half that number.
 * Every leaf has as many pointers to data as keys,
 * and every internal node has one more pointer
 * to a subtree than the number of keys.
 * This global variable is initialized to the
 * default value.
 */
//왜 여기선 차수-1개만큼만 가질수있지? 공간을 좀더 넣어서 효율성을높이나?
extern int order;

/* The queue is used to print the tree in
 * level order, starting from the root
 * printing each entire rank on a separate
 * line, finishing with the leaves.
 */
//queue 는 루트부터 레벨순서로 쭈욱 리프까지 출력하려고 만든거
extern node * queue;

/* The user can toggle on and off the "verbose"
 * property, which causes the pointer addresses
 * to be printed out in hexadecimal notation
 * next to their corresponding keys.
 */
//verbose는 포인터어드레스를 같이 출력해주려고?...
extern bool verbose_output;


// FUNCTION PROTOTYPES.

// Output and utility.

void license_notice( void );
void print_license( int licence_part );
void usage_1( void );
void usage_2( void );
void usage_3( void );
void enqueue( node * new_node );
node * dequeue( void );
int height( node * root );
int path_to_root( node * root, node * child );
void print_leaves( node * root );
void print_tree( node * root );
void find_and_print(node * root, int key, bool verbose); 
void find_and_print_range(node * root, int range1, int range2, bool verbose); 
int find_range( node * root, int key_start, int key_end, bool verbose,
        int returned_keys[], void * returned_pointers[]); 
node * find_leaf( node * root, int key, bool verbose );
record * find( node * root, int key, bool verbose );림
int cut( int length );

// Insertion.

record * make_record(int value);
node * make_node( void );
node * make_leaf( void );
int get_left_index(node * parent, node * left);
node * insert_into_leaf( node * leaf, int key, record * pointer );
node * insert_into_leaf_after_splitting(node * root, node * leaf, int key,
                                        record * pointer);
node * insert_into_node(node * root, node * parent, 
        int left_index, int key, node * right);
node * insert_into_node_after_splitting(node * root, node * parent,
                                        int left_index, int key, node * right);
node * insert_into_parent(node * root, node * left, int key, node * right);
node * insert_into_new_root(node * left, int key, node * right);
node * start_new_tree(int key, record * pointer);
node * insert( node * root, int key, int value );

// Deletion.

int get_neighbor_index( node * n );
node * adjust_root(node * root);
node * coalesce_nodes(node * root, node * n, node * neighbor,
                      int neighbor_index, int k_prime);
node * redistribute_nodes(node * root, node * n, node * neighbor,
                          int neighbor_index,
        int k_prime_index, int k_prime);
node * delete_entry( node * root, node * n, int key, void * pointer );
node * delete( node * root, int key );

void destroy_tree_nodes(node * root);
node * destroy_tree(node * root);

#endif /* __BPT_H__*/