#include "Queue.h" 
struct queue {
	struct queue *prev;
	struct queue *next;
	void *data;
};
static void queue_first(queue_s **);
static void queue_prev(queue_s **);
static void queue_last(queue_s **);
static void queue_next(queue_s **);
queue_s *queue_new(void) {
	return (NULL);
}
void queue_post(queue_s ** pp_queue, void *data) {
	if (pp_queue != NULL) {
		queue_s *p_l= NULL;
		queue_s *p_p= NULL;
		queue_first(pp_queue);
		p_l = *pp_queue;
		p_p = malloc(sizeof (*p_p));
		if (p_p != NULL) {
			p_p->data = data;
			p_p->next = p_l;
			p_p->prev = NULL;
			if (p_l != NULL)
				p_l->prev = p_p;
			*pp_queue = p_p;
		} else {
			fprintf(stderr, "Memoire insuffisante\n");
			exit(EXIT_FAILURE);
		}
	}
	return;
}
void *queue_get(queue_s ** pp_queue) {
	void *ret= NULL;
	if (pp_queue != NULL && *pp_queue != NULL) {
		queue_s *p_l= NULL;
		queue_s *p_p= NULL;
		queue_last(pp_queue);
		p_l = *pp_queue;
		if (p_l != NULL)
			p_p = p_l->prev;
		ret = p_l->data;
		free(p_l);
		p_l = NULL;
		if (p_p != NULL)
			p_p->next = NULL;
		*pp_queue = p_p;
	}
	return (ret);
}
void queue_delete(queue_s ** pp_queue) {
	if (pp_queue != NULL && *pp_queue != NULL) {
		while (*pp_queue != NULL)
			queue_get(pp_queue);
	}
	return;
}
static void queue_first(queue_s ** pp_queue) {
	if (pp_queue != NULL && *pp_queue != NULL) {
		while ((*pp_queue)->prev != NULL)
			queue_prev(pp_queue);
	}
	return;
}
static void queue_prev(queue_s ** pp_queue) {
	if (pp_queue != NULL && *pp_queue != NULL)
		*pp_queue = (*pp_queue)->prev;
	return;
}
static void queue_last(queue_s ** pp_queue) {
	if (pp_queue != NULL && *pp_queue != NULL) {
		while ((*pp_queue)->next != NULL)
			queue_next(pp_queue);
	}
	return;
}
static void queue_next(queue_s ** pp_queue) {
	if (pp_queue != NULL && *pp_queue != NULL)
		*pp_queue = (*pp_queue)->next;
	return;
}
