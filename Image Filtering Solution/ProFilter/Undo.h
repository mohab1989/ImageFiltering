#pragma once
struct Undo_type 
{		
	// Linked list of image buffers for undo
	int *p;				// Ptr to pixel buffer
	Undo_type *next;	// Ptr to next node
};

class CMyImage : public CImage 
{
	public:
	Undo_type *undo;			// Head ptr to image undo stack
};


class Undo
{
public:

	Undo();
	~Undo(void);
	void OnDo(CMyImage &image);
	void OnUnDo(CMyImage &image);
	void FreeUnDo(CMyImage &image);
	int undoCounter;
};
