#include "stdafx.h"
#include "Undo.h"


Undo::Undo()
{
	undoCounter=0;
}


Undo::~Undo(void)
{
}



//-----------------------------------------------------------------------
// Inserts a copy of the image onto the undo stack. 
//-----------------------------------------------------------------------
void Undo::OnDo(CMyImage &image)
{
	undoCounter +=1;
	int *p, *buf;	// Image ptrs
	Undo_type *h;

	unsigned long n = image.GetWidth()*image.GetHeight();

	// Allocate n sized block of memory from the heap for bitmap
	if (!(buf = (int *)malloc(n * sizeof(int)))) {
		ATLTRACE2("***"__FUNCTION__" malloc() error for size: %d\n", n);
		return;
	}

	// Allocate new link for Undo stack
	h = (Undo_type *) malloc(sizeof(Undo_type)); // New link
	if (!h) {
		ATLTRACE2("***"__FUNCTION__" malloc() error for size: %d\n", sizeof(Undo_type));
		free(buf);
		return;
	}

	p = (int *) image.GetBits();	// Ptr to bitmap
	memcpy_s(buf, n * sizeof(int), p, n * sizeof(int)); // Copy bitmap to buf

	h->p = buf;						// Save ptr to buf

	h->next = image.undo;	// Points to old head ptr.
	image.undo = h;		// New head ptr.

	return;
}


//-----------------------------------------------------------------------
// Pops previous state of image off the undo stack and restores image. 
//-----------------------------------------------------------------------
void Undo::OnUnDo(CMyImage &image)
{
		undoCounter-=1;
		int *p;				// Image ptr
		Undo_type *h;
		unsigned long n, nx = 0, ny = 0;
		n= image.GetWidth()*image.GetHeight();
		h = image.undo;	// Head ptr.
		p = (int *) image.GetBits();	// Ptr to bitmap

		// Restore bitmap
		memcpy_s(p, n * sizeof(int), h->p, n * sizeof(int)); 
	

		image.undo = h->next;		// Save new head ptr.

		free(h->p);					// Free pixel buffer
		free(h);					// Free undo ptr.

		return;
}

//-----------------------------------------------------------------------
// Function frees the undo stack
//-----------------------------------------------------------------------
void Undo::FreeUnDo(CMyImage &image)
{
	undoCounter=0;
	Undo_type *i, *j;

	for (i = image.undo; i; i = j) {
		free(i->p);
		j = i->next;
		free(i);
	}
	image.undo = NULL;
	return;
}