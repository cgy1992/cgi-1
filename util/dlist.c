#include "dlist.h"


void dlistInit(dlist *dlist,int (*match)(const void*,const void*),void (*destroy)(void *data)){
    dlist->size = 0;
    dlist->match = match;
    dlist->destroy = destroy;
    dlist->head = NULL;
    dlist->tail = NULL;
}

int dlistRemove(dlist *dlist,dlistelmt *element,void **data){
    if (element == NULL || dlist->size == 0){
        return -1;
    }    
    *data = element->data;
    if (element == dlist->head){
        dlist->head = element->next;
        if (dlist->size == 1){
            dlist->tail = NULL;    
        } else {
            element->next->prev = NULL;
        }        
    } else if (element == dlist->tail){
        dlist->tail = element->prev;
        if (dlist->size == 1){
            dlist->head = NULL;       
        } else {
           element->prev->next = NULL; 
        } 
    } else {
        element->prev->next = element->next;
        element->next->prev = element->prev;
    }
    free(element);
    dlist->size--;
    return 0;
}

void dlistDestroy(dlist *dlist){
    void *data;
    while(dlist->size > 0){
        dlistRemove(dlist,dlist->head,(void **)&data);
        dlist->destroy(data);
    }
    memset(dlist,0,sizeof(dlist));
    return;
}

int dlistInsert(dlist *dlist,dlistelmt *element,const void *data){
    if (NULL == dlist){
        return -1;
    }    
    dlistelmt *newelmt = (dlistelmt *)malloc(sizeof(dlistelmt));
    if (NULL == newelmt){
        return -1;
    }
    newelmt->data = (void *)data;
    if (0 == dlist->size){
        dlist->head = newelmt;
        dlist->tail = newelmt;
    } else if (1 == dlist->size){
        if (NULL == element){
            dlist->head = newelmt;
            newelmt->next = dlist->tail;
            dlist->tail->prev = newelmt;
        } else {
            dlist->head->next = newelmt;
            newelmt->prev = dlist->head;
            dlist->tail = newelmt;
        }
    } else {
        if (NULL == element){
            dlist->head->prev = newelmt;
            newelmt->next = dlist->head;
            dlist->head = newelmt; 
        } else if (element == dlist->tail){
            dlist->tail->next = newelmt;
            newelmt->prev = dlist->tail;
            dlist->tail = newelmt;
        } else {
            newelmt->prev = element;
            newelmt->next = element->next;
            element->next->prev = newelmt;
            element->next = newelmt;
        } 
    }
    dlist->size++;
    return 0;
}
