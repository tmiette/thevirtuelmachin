#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdio.h> 
#include <stdlib.h> 

typedef struct queue queue_s; 
queue_s *queue_new (void); 
void queue_post (queue_s **, void *); 
void *queue_get (queue_s **); 
void queue_delete (queue_s **); 

#endif /*QUEUE_H_*/
