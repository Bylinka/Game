#pragma once
template<class t>
class simple_ptr {
	size_t count;
	t* object;
public:
	simple_ptr(t* object)
		: object(object)
		, count(1)
	{
//		cout << "simple_ptr create " << this << endl;
	}
	simple_ptr()
		: object(NULL)
		, count(0)
	{
//		cout << "simple_ptr create " << this << endl;

	}
	bool empty() {
		return !count;
	}
	~simple_ptr() {
		if (count)count--;
		if (!count)delete object;
//		cout << "simple_ptr release" << this << endl;
	}
	void reset(t* object = NULL) {
		if (count) {
			delete this->object;
			count = 0;
		}
		if (object) {
			this->object = object;
			count++;
		}
	}
	t* operator->() {
		return object;
	}
};
