#include<atomic>

typedef unsigned long int UINT64;

class AtomicMarkableReference
{

public:
	AtomicMarkableReference(UINT64 reference, bool mark);
	~AtomicMarkableReference();

	bool compareAndSet(UINT64 expectedReference, UINT64 newReference, bool expectedMark, bool newMark);
	bool attemptMark(UINT64 expected, bool newMark);
	UINT64 get(bool* pMarked);
	UINT64 getRef();

	std::atomic<UINT64> addrAndFlag;
};

AtomicMarkableReference::AtomicMarkableReference(UINT64 reference, bool mark)
{
	reference = (mark)?(reference | 0x8000000000000000):(reference | 0x0000000000000000);
	addrAndFlag.store(reference);
}

AtomicMarkableReference::~AtomicMarkableReference()
{
}

bool AtomicMarkableReference::compareAndSet(UINT64 expectedReference, UINT64 updatedReference, bool expectedMark, bool updatedMark)
{
	UINT64 expected = (expectedMark)?(expectedReference | 0x8000000000000000):(expectedReference | 0x0000000000000000);
	UINT64 updated = (updatedMark)?(updatedReference | 0x8000000000000000):(updatedReference | 0x0000000000000000);
	return addrAndFlag.compare_exchange_strong(expected,updated);
}

bool AtomicMarkableReference::attemptMark(UINT64 expectedReference, bool newMark)
{
	UINT64 updatedReference = (newMark)?(expectedReference | 0x8000000000000000):(expectedReference | 0x0000000000000000);
	return addrAndFlag.compare_exchange_strong(expectedReference,updatedReference);
}

UINT64 AtomicMarkableReference::get(bool* pMarked)
{
	UINT64 retVal = addrAndFlag.load();
	*pMarked = retVal & 0x8000000000000000;
	retVal = retVal & 0x0fffffffffffffff;
	return retVal;
}

UINT64 AtomicMarkableReference::getRef()
{
	UINT64 retVal = addrAndFlag.load();
	retVal = retVal & 0x0fffffffffffffff;
	return retVal;
}


