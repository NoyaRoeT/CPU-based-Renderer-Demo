#pragma once
template <typename T>

struct Buffer
{
public:
	Buffer(int width, int height) : mWidth(width), mHeight(height), mPitch(width * sizeof(T)), mArray(new T[width * height]) {}

	T& operator() (int x, int y) { return mArray[x + y * mWidth]; }
	const T& operator() (int x, int y) const { return mArray[x + y * mWidth]; }

	void Clear(const T& s) 
	{
		for (int i = 0; i != mWidth * mHeight; ++i)
		{
			mArray[i] = s;
		}
	}


	~Buffer() { delete[] mArray; }
public:
	int mWidth;
	int mHeight;
	int mPitch;
	T* mArray;
};