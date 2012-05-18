#pragma once 

#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <functional>
#include <type_traits>

int Count = 0;

namespace Linq{

class InvalidOperationException{};

template<typename T>
class Linq;

template<typename T>
class LinqOrd;


template<typename T>  
class Range
{
public:
	Range()	{ Count++; }
	virtual ~Range() {Count --;}
	virtual bool empty() = 0;
	virtual T& popFront() = 0;
	virtual T& front() = 0;
	virtual void rewind() = 0;
	virtual Range* clone() = 0;
};

//=============================================================================
template<typename T>
class Linq
{
public:
	Linq();
	Linq(const Linq &rh);
	Linq(Linq &&rh);
	Linq& operator=(const Linq &rh);
	Linq& operator=(Linq &&rh);
	virtual ~Linq();

	T value();
	T& reference();
	const T& const_reference();

	template<typename F>
	bool all(F f);

	template<typename F>
	bool any(F f);

	template<typename F>
	bool contains(T val, F f);
	bool contains(T val);

	template<typename F>
	size_t count(F f);
	size_t count();

	Linq<typename T> concat(const Linq<typename T> &second);
	
	Linq<typename T> defaultIfEmpty();
	Linq<typename T> defaultIfEmpty(const T &defaultValue);

	template<typename F>
	Linq<typename T> distinct(F f);
	Linq<typename T> distinct();

	template <typename U>
	Linq<typename T> where(U f);
	
	template <typename U>
	LinqOrd<typename T> orderBy(U f);
	template <typename U, typename Comp>
	LinqOrd<typename T> orderBy(U f, Comp comp);

	template <typename U>
	LinqOrd<typename T> orderByDesc(U f);
	template <typename U, typename Comp>
	LinqOrd<typename T> orderByDesc(U f, Comp comp);

	template <typename U>
	auto select(U f) -> Linq<decltype(f(MakeType<T>()))>;
	
	template <typename F>
	Linq<typename T> skip(F f);
	Linq<typename T> skip(int n);

	Linq<typename T> take(unsigned int n);

	template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
	auto join(
		Linq<U> other, 
		FLeftKey leftKey, 
		FRightKey rightKey, 
		FResult resultSelector
		) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;

	template<typename U, typename FLeftKey, typename FRightKey, typename FResult, typename FKeyComp>
	auto join(
		Linq<U> other, 
		FLeftKey leftKey, 
		FRightKey rightKey, 
		FResult resultSelector, 
		FKeyComp keyComp
		) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;
    
    template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
	auto leftJoin(
		Linq<U> other, 
		FLeftKey leftKey, 
		FRightKey rightKey, 
		FResult resultSelector,
        U defaultValue
        ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;

    template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
	auto fullJoin(
		Linq<U> other, 
		FLeftKey leftKey, 
		FRightKey rightKey, 
		FResult resultSelector,
        T defaultLValue,
        U defaultRValue
        ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;

	template<typename F>
	auto selectMany(F f) -> Linq< decltype(f(MakeType<T>()).value()) >;
	
	template<typename F, typename G>
	auto groupBy(F f, G g) -> Linq<
		std::pair<
			typename std::remove_const<decltype(f(MakeType<T>()))>::type, 
			std::vector<typename std::remove_const<decltype(g(MakeType<T>()))>::type>
			>
		>;

	template<typename F>
	auto groupBy(F f) -> Linq<
		std::pair<
			typename std::remove_const<decltype(f(MakeType<T>()))>::type, 
			std::vector<T>
			>
		>;

	T first();

	T firstOrDefault();

	T single();

	template<typename F>
	T elect(F f);

	T min();

	T max();

	std::vector<typename T> toVector();
	std::set<typename T> toSet();	
	std::list<typename T> toList();	
	
	template<typename FKey, typename FValue>
	auto toMap(FKey key, FValue value) -> std::map<decltype(key(MakeType<T>())), decltype(value(MakeType<T>()))>;
public:
	Range<T> *range;
};
//=============================================================================
template<typename T>
class LinqOrd: public Linq<typename T>
{
public:
	LinqOrd();
	LinqOrd(const LinqOrd &rh);
	LinqOrd(LinqOrd &&rh);
	LinqOrd& operator=(const LinqOrd &rh);
	LinqOrd& operator=(LinqOrd &&rh);
	
	template <typename U>
	LinqOrd<typename T> thenBy(U f);
	template <typename U, typename Comp>
	LinqOrd<typename T> thenBy(U f, Comp comp);
	template <typename U>
	LinqOrd<typename T> thenByDesc(U f);
	template <typename U, typename Comp>
	LinqOrd<typename T> thenByDesc(U f, Comp comp);
};
//=============================================================================

namespace Implemenatation {

template<typename T>  
Range<T>* CloneRange(Range<T> *src)
{
	if (src == nullptr)
		return nullptr;
	return src->clone();
}

template<typename TIter> 
class IteratorRange : public Range<typename std::iterator_traits<TIter>::value_type>
{
public:
	typedef typename std::iterator_traits<TIter>::value_type T;

	IteratorRange(TIter b, TIter e)
		: b_(b)
		, e_(e)
		, iter_(b)
    {
    }

    bool empty() override
    {
        return (iter_ == e_);
    }

    T& popFront() override
    {
		if (empty())
			throw new InvalidOperationException();
        return const_cast<T&>(*(iter_++));
    }

    T& front() override
    {
        if (empty())
			throw new InvalidOperationException();
        return const_cast<T&>(*iter_);
    }

	void rewind() override
	{
		iter_ = b_;
	}

	Range* clone() override
	{
		auto result = new IteratorRange(b_, e_);
		return result;
	}
private:
    TIter b_;
    TIter e_;
	TIter iter_;
};

template<typename TCont, typename TValue> 
class ContainerRange : public Range<typename TValue>
{
public:
	//typedef typename TCont::value_type T;
	typedef typename TValue T;
	typedef typename TCont::iterator TIter;

	ContainerRange(TCont &cont)
		: cont_(cont)
		, e_(std::end(cont))
		, iter_(std::begin(cont))
    {
    }

    bool empty() override
    {
        return (iter_ == e_);
    }

    T& popFront() override
    {
        if (empty())
			throw new InvalidOperationException();
        return *(iter_++);
    }

    T& front() override
    {
		if (empty())
			throw new InvalidOperationException();
        return *iter_;
    }

	virtual void rewind() override
	{
		iter_ = std::begin(cont_);
		e_ = std::end(cont_);
	}

	Range* clone() override
	{
		auto result = new ContainerRange(cont_);
		return result;
	}
private:
	TCont &cont_;
    TIter e_;
	TIter iter_;
};

template<typename T>  
class ConcatRange : public Range<T>	
{
public:
	ConcatRange(Range<T> *first, Range<T> *second)
		: first_(first)
		, second_(second)
		, firstPart_(true)
	{
	}

	~ConcatRange()
	{
		delete first_;
		delete second_;
	}

	bool empty() override
	{
		if (!firstPart_)
			return second_->empty();
		auto res= first_->empty();
		if (res == false)
			return false;
		firstPart_ = false;
		return second_->empty();
	}

	T& popFront() override
	{
		if (firstPart_)
			return first_->popFront();
		else
			return second_->popFront();
	}

	T& front() override
	{
		if (firstPart_)
			return first_->front();
		else
			return second_->front();
	}

	void rewind() override
	{
		first_->rewind();
		second_->rewind();
		firstPart_ = true;
	}

	Range* clone() override
	{
		auto result = new ConcatRange<T>(CloneRange(first_), CloneRange(second_));
		return result;
	}
private:
	Range<T> *first_;
	Range<T> *second_;
	bool firstPart_;
};

template<typename T>  
class DefaultIfEmptyRange : public Range<T>	
{
public:
	DefaultIfEmptyRange(Range<T> *src, const T &defaultValue)
		: src_(src)
		, isPrepared_(false)
		, isSrcEmpty_(true)
		, atEnd_(false)
		, defaultValue_(defaultValue)
	{
	}

	~DefaultIfEmptyRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (!isPrepared_)
			prepare();
		
		if (!isSrcEmpty_)
			return src_->empty();
		return atEnd_;
	}

	T& popFront() override
	{
		if (!isPrepared_)
			prepare();
		if (!isSrcEmpty_)
			return src_->popFront();
		if (atEnd_)
			throw new InvalidOperationException();
		atEnd_ = true;
		return defaultValue_;
	}

	T& front() override
	{
		if (!isPrepared_)
			prepare();
		if (!isSrcEmpty_)
			return src_->front();
		if (atEnd_)
			throw new InvalidOperationException();
		return defaultValue_;
	}

	void rewind() override
	{
		src_->rewind();
		isPrepared_ = false;
	}

	Range* clone() override
	{
		auto result = new DefaultIfEmptyRange<T>(CloneRange(src_), defaultValue_);
		return result;
	}
private:
	Range<T> *src_;
	T defaultValue_;
	bool isPrepared_;
	bool atEnd_;
	bool isSrcEmpty_;

	void prepare()
	{
		isPrepared_ = true;
		isSrcEmpty_ = src_->empty();
		atEnd_ = false;
	}
};

template<typename T, typename F>  
class WhereRange : public Range<T>	
{
public:
	WhereRange(Range<T> *src, F f)
		: src_(src)
		, f_(f)
		, frontReady(false)
	{
	}

	~WhereRange()
	{
		delete src_;
	};

	bool empty() override
	{
		if (!frontReady)
            seekFront();
        return src_->empty();
	}

	T& popFront() override
	{
		if (!frontReady)
            seekFront();
		auto &result = src_->front();
		frontReady = false;
		src_->popFront();
		seekFront();
		return result;
	}

	T& front() override
	{
		return src_->front();
	}
		
	void rewind() override
	{
		frontReady = false;
		src_->rewind();
	}

	Range* clone() override
	{
		auto result = new WhereRange(CloneRange(src_), f_);
		return result;
	}
private:
	Range<T> *src_;
	F f_;
	bool frontReady;

	void seekFront()
    {
        while(!src_->empty() && !f_(src_->front()))
            src_->popFront();
        frontReady = true;
    }
};

template<typename T>  
class BaseOrderByRange : public Range<T>	
{
public:
	typedef std::function<int(const T *, const T *)> Comparator;

	template<typename F, typename Comp>
	void addComparator(F f, Comp comp, bool isDesc)
	{
		if (!isDesc)
		{
			auto comparator = 
				[=]
				(const T *lh, const T *rh) -> int 
				{
					if (comp(f(*lh), f(*rh)))
						return -1;
					else if (comp(f(*rh), f(*lh)))
						return 1;
					else
						return 0;
				};
			comparators.push_back(comparator);
		}
		else
		{
			auto comparator = 
				[=]
				(const T *lh, const T *rh) -> int 
				{
					if (comp(f(*lh), f(*rh)))
						return 1;
					else if (comp(f(*rh), f(*lh)))
						return -1;
					else
						return 0;
				};
			comparators.push_back(comparator);
		}
	}
protected:
	std::vector<Comparator> comparators;

};

template<typename T, typename F, typename Comp>  
class OrderByRange : public BaseOrderByRange<T>	
{
public:
	OrderByRange(Range<T> *src, F f, Comp comp, bool isDesc)
		: src_(src)
		, f_(f)
		, comp_(comp)
		, isDesc_(isDesc)
		, prepared_(false)
	{
		addComparator(f, comp, isDesc);
	}

	~OrderByRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (!prepared_)
			prepare();
		return iter_ == orderedData_.end();
	}

	T& popFront() override
	{
		if (!prepared_)
			prepare();
		T* v = *(iter_++);
		T& vv = *v;
		return vv;
	}

	T& front() override
	{
		T* v = *(iter_);
		T& vv = *v;
		return vv;
	}

	void rewind() override
	{
		src_->rewind();
		prepared_ = false;
		orderedData_.clear();
		iter_ = orderedData_.begin();
	}

	Range* clone() override
	{
		auto result = dynamic_cast<OrderByRange*>(new OrderByRange(CloneRange(src_), f_, comp_, isDesc_));
		result->comparators = comparators;
		return result;
	}
private:
	Range<T> *src_;
	F f_;
	Comp comp_;
	bool isDesc_;
	bool prepared_;
	typename std::vector< T*> orderedData_;
	typename std::vector< T*>::iterator iter_;

	void prepare()
    {
		while(!src_->empty())
			orderedData_.push_back(&src_->popFront());

		std::function<bool(const T *lh, const T *rh)> aggComp;
		if (comparators.size() == 1)
		{
			aggComp = 
				[=]
				(const T *lh, const T *rh) -> bool
				{
					return (!isDesc_) ? 
						comp_(f_(*lh), f_(*rh)) :
						comp_(f_(*rh), f_(*lh)) ;
				};
		}
		else
		{
			aggComp = 
				[&]
				(const T *lh, const T *rh) -> bool
				{
					for (auto ii = comparators.begin(); ii != comparators.end(); ++ii)
					{
						int c = (*ii)(lh, rh);
						if (c != 0)
							return c < 0;
					}
					return false;
				};
		}

		std::sort(
			orderedData_.begin(), 
			orderedData_.end(),
			aggComp
		);

		iter_ = orderedData_.begin();
        prepared_ = true;
    }
};

template<typename T, typename U, typename F>  
class SelectRange : public Range<T>	
{
public:
	SelectRange(Range<U> *src, F f)
		: src_(src)
		, f_(f)
		, prepared_(false)
	{
	}

	~SelectRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (!prepared_)
			prepare();
		return iter_ == data_.end();
	}

	T& popFront() override
	{
		if (!prepared_)
			prepare();
		return *(iter_++);
	}

	T& front() override
	{
		return  *(iter_);
	}

	void rewind() override
	{
		prepared_ = false;
		src_->rewind();
		data_.clear();
		iter_ = data_.begin();
	}

	Range* clone() override
	{
		auto result = new SelectRange<T, U, F>(CloneRange(src_), f_);
		return result;
	}
private:
	Range<U> *src_;
	F f_;
	bool prepared_;
	typename std::vector<T> data_;
	typename std::vector<T>::iterator iter_;

	void prepare()
    {
		prepared_ = true;
		if (src_->empty())
			return;

		while(!src_->empty())
		{
			data_.push_back(f_(src_->popFront()));
		}
		iter_ = data_.begin();
    }
};

template<typename T>  
class SkipRange : public Range<T>	
{
public:
	SkipRange(Range<T> *src, unsigned int n)
		: src_(src)
		, n_(n)
		, prepared_(false)
	{
	}

	~SkipRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (!prepared_)
			prepare();
		return src_->empty();
	}

	T& popFront() override
	{
		if (!prepared_)
			prepare();
		return src_->popFront();
	}

	T& front() override
	{
		return src_->front();
	}

	void rewind() override
	{
		src_->rewind();
		prepared_ = false;
	}

	Range* clone() override
	{
		auto result = new SkipRange<T>(CloneRange(src_), n_);
		return result;
	}
private:
	Range<T> *src_;
	bool prepared_;
	unsigned int n_;

	void prepare()
    {
		unsigned int skipedCount = 0;
		while(!src_->empty() && skipedCount < n_)
		{
			src_->popFront();
			skipedCount++;
		}
        prepared_ = true;
    }
};

template<typename T, typename F>  
class SkipWhileRange : public Range<T>	
{
public:
	SkipWhileRange(Range<T> *src, F f)
		: src_(src)
		, f_(f)
		, prepared_(false)
	{
	}

	~SkipWhileRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (!prepared_)
			prepare();
		return src_->empty();
	}

	T& popFront() override
	{
		if (!prepared_)
			prepare();
		return src_->popFront();
	}

	T& front() override
	{
		return src_->front();
	}

	void rewind() override
	{
		src_->rewind();
		prepared_ = false;
	}

	Range* clone() override
	{
		auto result = new SkipWhileRange<T, F>(CloneRange(src_), f_);
		return result;
	}
private:
	Range<T> *src_;
	bool prepared_;
	F f_;

	void prepare()
    {
		while((!src_->empty()) && (f_(src_->front()))
			)
		{
			src_->popFront();
		}
        prepared_ = true;
    }
};

template<typename T>  
class TakeRange : public Range<T>	
{
public:
	TakeRange(Range<T> *src, unsigned int n)
		: src_(src)
		, n_(n)
		, taken_(0)
	{
	}

	~TakeRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (taken_ >= n_)
			return true;
		return src_->empty();
	}

	T& popFront() override
	{
		if (taken_ >= n_)
			throw new InvalidOperationException();
		taken_++;
		return src_->popFront();
	}

	T& front() override
	{
		if (taken_ > n_)
			throw new InvalidOperationException();
		return src_->front();
	}

	void rewind() override
	{
		taken_ = 0;
		src_->rewind();
	}

	Range* clone() override
	{
		auto result = new TakeRange<T>(CloneRange(src_), n_);
		return result;
	}
private:
	Range<T> *src_;
	unsigned int taken_;
	unsigned int n_;
};

template<typename LT, typename RT, typename TResult>  
class BaseInnerJoinRange : public Range<TResult>	
{
public:
	typedef TResult T;

	BaseInnerJoinRange(Range<LT> *srcL, Range<RT> *srcR)
		: srcL_(srcL)
		, srcR_(srcR)
		, prepared_(false)
	{
	}

	~BaseInnerJoinRange()
	{
		delete srcL_;
		delete srcR_;
	}

	bool empty() override
	{
		if (!prepared_)
			prepare();
		return iter_ == data_.end();
	}

	T& popFront() override
	{
		if (!prepared_)
			prepare();
		return *(iter_++);
	}

	T& front() override
	{
		return  *(iter_);
	}

	void rewind() override 
	{
		srcL_->rewind();
		srcR_->rewind();
		prepared_ = false;
		data_.clear();
		iter_ = data_.begin();
	}
protected:
	Range<LT> *srcL_;
	Range<RT> *srcR_;
	bool prepared_;
	typename std::vector<T> data_;
	typename std::vector<T>::iterator iter_;

	virtual void prepare() = 0;
};

template<typename LT, typename RT, typename FLeftKey, typename FRightKey, typename FResult, typename TResult, typename FKeyComp>
class JoinRange : public BaseInnerJoinRange<LT, RT, TResult>
{
public:
	JoinRange(Range<LT> *srcL, Range<RT> *srcR, FLeftKey leftKey, FRightKey rightKey, FResult selectResult, FKeyComp keyComp)
		: BaseInnerJoinRange(srcL, srcR)
		, leftKey_(leftKey)
		, rightKey_(rightKey)
		, selectResult_(selectResult)
		, keyComp_(keyComp)
	{
	}

	Range* clone() override
	{
		auto result = new JoinRange<LT, RT, FLeftKey, FRightKey, FResult, TResult, FKeyComp>(
			CloneRange(srcL_), 
			CloneRange(srcR_), 
			leftKey_,
			rightKey_,
			selectResult_,
			keyComp_
			);
		return result;
	}
protected:
	void prepare()
    {
		prepared_ = true;
		if (srcL_->empty() || srcR_->empty())
			return;

		//�������������� ������ ����-�������� �� ������� ������
		typedef std::pair<decltype(rightKey_(MakeType<RT>())), RT*> KeyValue;
		std::vector<KeyValue> rhData;
		while(!srcR_->empty())
		{
			auto pVal = &srcR_->popFront();
			rhData.push_back(KeyValue(rightKey_(*pVal), pVal));
		}

		//��������� �� ������ ������
		while(!srcL_->empty())
		{
			//��������� ��������
			LT *lh = &srcL_->popFront();
			//� ����
			auto lhKey = leftKey_(*lh);
			
			//������ ���� � ������ ������ ����� �� �����
			for(auto ii = rhData.begin() ; ii != rhData.end(); ++ii)	
			{
				//���������� ����� �� ���������������
				if (keyComp_(lhKey, ii->first))
					data_.push_back(selectResult_(*lh, *(ii->second)));
			}
		}
		iter_ = data_.begin();
    }
private:
	FLeftKey leftKey_;
	FRightKey rightKey_;
	FResult selectResult_;
	FKeyComp keyComp_;
};

template<typename LT, typename RT, typename FLeftKey, typename FRightKey, typename FResult, typename TResult, typename FKeyComp>
class LeftJoinRange : public BaseInnerJoinRange<LT, RT, TResult>
{
public:
	LeftJoinRange(Range<LT> *srcL, Range<RT> *srcR, FLeftKey leftKey, FRightKey rightKey, FResult selectResult, FKeyComp keyComp, RT defaultValue)
		: BaseInnerJoinRange(srcL, srcR)
		, leftKey_(leftKey)
		, rightKey_(rightKey)
		, selectResult_(selectResult)
		, keyComp_(keyComp)
        , defaultValue_(defaultValue)
	{
	}

	Range* clone() override
	{
		auto result = new LeftJoinRange<LT, RT, FLeftKey, FRightKey, FResult, TResult, FKeyComp>(
			CloneRange(srcL_), 
			CloneRange(srcR_), 
			leftKey_,
			rightKey_,
			selectResult_,
			keyComp_,
            defaultValue_
			);
		return result;
	}
protected:
	void prepare()
    {
		prepared_ = true;
		if (srcL_->empty())
			return;

		//�������������� ������ ����-�������� �� ������� ������
		typedef std::pair<decltype(rightKey_(MakeType<RT>())), RT*> KeyValue;
		std::vector<KeyValue> rhData;
		while(!srcR_->empty())
		{
			auto pVal = &srcR_->popFront();
			rhData.push_back(KeyValue(rightKey_(*pVal), pVal));
		}

		//��������� �� ������ ������
		while(!srcL_->empty())
		{
			//��������� ��������
			LT *lh = &srcL_->popFront();
			//� ����
			auto lhKey = leftKey_(*lh);
			
            bool isFound = false;
			//������ ���� � ������ ������ ����� �� �����
			for(auto ii = rhData.begin() ; ii != rhData.end(); ++ii)	
			{
				//���������� ����� �� ���������������
				if (keyComp_(lhKey, ii->first))
                {
					data_.push_back(selectResult_(*lh, *(ii->second)));
                    isFound = true;
                }
			}

            if (!isFound)
                data_.push_back(selectResult_(*lh, defaultValue_));
		}
		iter_ = data_.begin();
    }
private:
	FLeftKey leftKey_;
	FRightKey rightKey_;
	FResult selectResult_;
	FKeyComp keyComp_;
    RT defaultValue_;
};

template<typename LT, typename RT, typename FLeftKey, typename FRightKey, typename FResult, typename TResult, typename FKeyComp>
class FullJoinRange : public BaseInnerJoinRange<LT, RT, TResult>
{
public:
	FullJoinRange(Range<LT> *srcL, Range<RT> *srcR, FLeftKey leftKey, FRightKey rightKey, FResult selectResult, FKeyComp keyComp, LT defaultLValue, RT defaultRValue)
		: BaseInnerJoinRange(srcL, srcR)
		, leftKey_(leftKey)
		, rightKey_(rightKey)
		, selectResult_(selectResult)
		, keyComp_(keyComp)
        , defaultLValue_(defaultLValue)
        , defaultRValue_(defaultRValue)
	{
	}

	Range* clone() override
	{
		auto result = new FullJoinRange<LT, RT, FLeftKey, FRightKey, FResult, TResult, FKeyComp>(
			CloneRange(srcL_), 
			CloneRange(srcR_), 
			leftKey_,
			rightKey_,
			selectResult_,
			keyComp_,
            defaultLValue_,
            defaultRValue_
			);
		return result;
	}
protected:
	void prepare()
    {
		prepared_ = true;

		//�������������� ������ ����-�������� �� ������� ������
        typedef decltype(rightKey_(MakeType<RT>())) TRKey;
		typedef std::pair<TRKey, RT*> RKeyValue;
		std::vector<RKeyValue> rhData;
		while(!srcR_->empty())
		{
			auto pVal = &srcR_->popFront();
			rhData.push_back(RKeyValue(rightKey_(*pVal), pVal));
		}

        std::set<TRKey> usedRKeys;
		//��������� �� ������ ������
		while(!srcL_->empty())
		{
			//��������� ��������
			LT *lh = &srcL_->popFront();
			//� ����
			auto lhKey = leftKey_(*lh);
			
            bool isFound = false;
			//������ ���� � ������ ������ ����� �� �����
			for(auto ii = rhData.begin() ; ii != rhData.end(); ++ii)	
			{
				//���������� ����� �� ���������������
				if (keyComp_(lhKey, ii->first))
                {
					data_.push_back(selectResult_(*lh, *(ii->second)));
                    isFound = true;
                    usedRKeys.insert(ii->first);
                }
			}

            if (!isFound)
                data_.push_back(selectResult_(*lh, defaultRValue_));
		}

        auto endIter = usedRKeys.end();
        for (auto ii = rhData.begin(), eii = rhData.end(); ii != eii; ++ii)
        {
            if (usedRKeys.find(ii->first) == endIter)
                //���� �� ����� ���� ����� ��������������, �� ��������� ������ ������ 
                data_.push_back(selectResult_(defaultLValue_, *(ii->second)));
        }

		iter_ = data_.begin();
    }
private:
	FLeftKey leftKey_;
	FRightKey rightKey_;
	FResult selectResult_;
	FKeyComp keyComp_;
    LT defaultLValue_;
    RT defaultRValue_;
};

template<typename T, typename U, typename F>  
class SelectManyRange : public Range<T>	
{
public:
	SelectManyRange(Range<U> *src, F f)
		: src_(src)
		, f_(f)
		, prepared_(false)
	{
	}

	~SelectManyRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (!prepared_)
			prepare();
		return src_->empty() || inner_.range->empty();
	}

	T& popFront() override
	{
		T &result = inner_.range->front();
		findNext();
		return result;
	}

	T& front() override
	{
		return  inner_.range->front();
	}

	void rewind() override 
	{
		src_->rewind();
		prepared_ = false;
	}

	Range* clone() override
	{
		auto result = new SelectManyRange<T, U, F>(
			CloneRange(src_), 
			f_
			);
		return result;
	}
private:
	Range<U> *src_;
	bool prepared_;
	F f_;

	Linq<typename T> inner_;

	void prepare()
	{
		prepared_ = true;
		while(!src_->empty())
		{
			//�������� ���������� ���������
			inner_ = f_(src_->front());
			inner_.range->rewind();
			//���� ��� �� ������, ������ ����� ������ ������������������
			if (!inner_.range->empty())
				return;
			//��������� � ���������� ��������
			src_->popFront();
		}
	}

	bool advanceInner()
	{
		if (inner_.range->empty())
			return false;
		inner_.range->popFront();
		return !inner_.range->empty();
	}

	void findNext()
	{
		while(!src_->empty())
		{
			if (advanceInner())
				return;
			src_->popFront();
			if (src_->empty())
				return;
			//�������� ���������� ���������
			inner_ = f_(src_->front());
			inner_.range->rewind();
			if (!inner_.range->empty())
				return;
		}
	}
};

template<typename T, typename F, typename TKey>  
class DistinctRange : public Range<T>	
{
public:
	DistinctRange(Range<T> *src, F f)
		: src_(src)
		, f_(f)
		, prepared_(false)
	{
	}

	~DistinctRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (!prepared_)
			prepare();
		return iter_ == end_;
	}

	T& popFront() override
	{
		T &result = *iter_->second;
		iter_++;
		return result;
	}

	T& front() override
	{
		return *iter_->second;
	}

	void rewind() override 
	{
		src_->rewind();
		prepared_ = false;
	}

	Range* clone() override
	{
		auto result = new DistinctRange<T, F, TKey>(
			CloneRange(src_), 
			f_
			);
		return result;
	}
private:
	Range<T> *src_;
	bool prepared_;
	F f_;
	std::map<typename TKey, typename T*> data_;
	typename std::map<typename TKey, typename T*>::iterator iter_;
	typename std::map<typename TKey, typename T*>::iterator end_;

	void prepare()
	{
		prepared_ = true;
		while(!src_->empty())
		{
			T &val = src_->popFront();
			TKey key = f_(val);
			if (data_.find(key) == data_.end())
				data_.insert(std::pair<typename TKey, typename T*>(key, &val));
		}
		iter_ = data_.begin();
		end_ = data_.end();
	}

};

template<typename T, typename TSrc, typename F, typename G, typename TKey, typename TValue>  
class GroupByRange : public Range<T>	
{
public:
	GroupByRange(Range<TSrc> *src, F f, G g)
		: src_(src)
		, f_(f)
		, g_(g)
		, prepared_(false)
	{
	}

	~GroupByRange()
	{
		delete src_;
	}

	bool empty() override
	{
		if (!prepared_)
			prepare();
		return iter_ == end_;
	}

	T& popFront() override
	{
		return *(iter_++);	
	}

	T& front() override
	{
		return *(iter_);
	}

	void rewind() override 
	{
		src_->rewind();
		prepared_ = false;
	}

	Range* clone() override
	{
		auto result = new GroupByRange<T, TSrc, F, G, TKey, TValue>(
			CloneRange(src_), 
			f_,
			g_
			);
		return result;
	}
private:
	Range<typename TSrc> *src_;
	bool prepared_;
	F f_;
	G g_;
	typedef std::vector<T> TData;
	TData data_;
	typename TData::iterator iter_;
	typename TData::iterator end_;

	void prepare()
	{
		prepared_ = true;
		typedef std::vector<typename TValue> TValues;
		typename std::map<typename TKey, typename TValues> tmp;

		while(!src_->empty())
		{
			TSrc &val = src_->popFront();
			TKey key = f_(val);
			tmp[key].push_back(g_(val));
		}

		for (auto ii = tmp.begin(); ii != tmp.end(); ++ii)
		{
			data_.push_back(T(ii->first, TValues()));
			data_.back().second.swap(ii->second);
		}

		iter_ = data_.begin();
		end_ = data_.end();
	}

};

}//Implemenatation
//=============================================================================
template<typename T>  
T MakeType();

template<typename T>
Linq<T>::Linq()
	:range(nullptr)
{
}

template<typename T>
Linq<T>::Linq(const Linq &rh)
{
	range = Implemenatation::CloneRange(rh.range);
}

template<typename T>
Linq<T>::Linq(Linq &&rh)
	:range(nullptr)
{
	std::swap(range, rh.range);
}

template<typename T>
typename Linq<T>& Linq<T>::operator=(const Linq &rh)
{
	if (this == &rh)
		return *this;
	delete range;
	range = Implemenatation::CloneRange(rh.range);
	return *this;
}

template<typename T>
typename Linq<T>& Linq<T>::operator=(Linq &&rh)
{
	std::swap(range, rh.range);
	return *this;
}

template<typename T>
Linq<T>::~Linq()
{
	delete range;
}
//-----------------------------------------------------------------------------
template<typename T>
template<typename F>
bool Linq<T>::all(F f)
{
	range->rewind();
	while(!range->empty())
	{
		if (! f(range->popFront()))
			return false;
	}
	return true;
}

template<typename T>
template<typename F>
bool Linq<T>::any(F f)
{
	range->rewind();
	while(!range->empty())
	{
		if (f(range->popFront()))
			return true;
	}
	return false;
}

template<typename T>
template<typename F>
bool Linq<T>::contains(T val, F f)
{
	range->rewind();
	while(!range->empty())
	{
		auto lh = range->popFront();
		if (f(lh, val))
			return true;
	}
	return false;
}

template<typename T>
bool Linq<T>::contains(T val) 
{
	return contains(val, [](const T& lh, const T& rh){ return lh == rh; });
}

template<typename T>
template<typename F>
size_t Linq<T>::count(F f)
{
	range->rewind();
	size_t cnt = 0;
	while(!range->empty())
	{
		auto lh = range->popFront();
		if (f(lh))
			cnt++;
	}
	return cnt;
}

template<typename T>
size_t Linq<T>::count()
{
	return count([](const T &){ return true; });
}

template<typename T>
Linq<typename T> Linq<T>::concat(const Linq<typename T> &second)
{
	Linq<typename T> result;
	result.range = new Implemenatation::ConcatRange<T>(Implemenatation::CloneRange(range), Implemenatation::CloneRange(second.range));
	return result;
}

template<typename T>
Linq<typename T> Linq<T>::defaultIfEmpty(const T &defaultValue)
{
	Linq<typename T> result;
	result.range = new Implemenatation::DefaultIfEmptyRange<T>(Implemenatation::CloneRange(range), defaultValue);
	return result;
}

template<typename T>
Linq<typename T> Linq<T>::defaultIfEmpty()
{
	return defaultIfEmpty(T());
}

template<typename T>
template<typename F>
Linq<typename T> Linq<T>::distinct(F f)
{
	Linq<T> result;
	result.range = new Implemenatation::DistinctRange<T, F, decltype(f(MakeType<T>()))>(Implemenatation::CloneRange(range), f);
	return result;
}

template<typename T>
Linq<typename T> Linq<T>::distinct()
{
	return distinct([](const T &a) { return a; });
}

template<typename T>
template<typename U>
Linq<typename T> Linq<T>::where(U f)
{
	Linq<typename T> result;
	result.range = new Implemenatation::WhereRange<T, U>(Implemenatation::CloneRange(range), f);
	return result;
}
	
template<typename T>
template<typename U>
LinqOrd<typename T> Linq<T>::orderBy(U f)
{
	typedef decltype(f(MakeType<T>())) TKey;
	auto comp = [](const TKey &lh, const TKey &rh) { return lh < rh ;};
	return orderBy(f, comp);
}

template<typename T>
template<typename U, typename Comp>
LinqOrd<typename T> Linq<T>::orderBy(U f, Comp comp)
{
	LinqOrd<typename T> result;
	result.range = new Implemenatation::OrderByRange<T, U, Comp>(Implemenatation::CloneRange(range), f, comp, false);
	return result;
}

template<typename T>
template<typename U>
LinqOrd<typename T> Linq<T>::orderByDesc(U f)
{
	typedef decltype(f(MakeType<T>())) TKey;
	auto comp = [](const TKey &lh, const TKey &rh) { return lh < rh; };
	return orderByDesc(f, comp);
}

template<typename T>
template<typename U, typename Comp>
LinqOrd<typename T> Linq<T>::orderByDesc(U f, Comp comp)
{
	typedef decltype(f(MakeType<T>())) TKey;
	LinqOrd<typename T> result;
	result.range = new Implemenatation::OrderByRange<T, U, decltype(comp)>(Implemenatation::CloneRange(range), f, comp, true);
	return result;
}

template<typename T>
template<typename U>
auto Linq<T>::select(U f) -> Linq<decltype(f(MakeType<T>()))>
{
	typedef decltype(f(MakeType<T>())) NewT;
	Linq<NewT> result;
	result.range = new Implemenatation::SelectRange<NewT, T, U>(Implemenatation::CloneRange(range), f);
	return result;
}

template<typename T>
template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
auto Linq<T>::join(
	Linq<U> other, 
	FLeftKey leftKey, 
	FRightKey rightKey, 
	FResult resultSelector
    ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<T>()))> 
{
	typedef decltype(resultSelector(MakeType<T>(), MakeType<U>())) NewT;
	typedef decltype(leftKey(MakeType<T>())) TLeftKey;
	typedef decltype(rightKey(MakeType<U>())) TRightKey;

	auto keyComp = 
		[]
		(const TLeftKey &lh, const TRightKey &rh) -> bool
		{
			return lh == rh;
		};

	typedef decltype(keyComp) FKeyComp;

	Linq<NewT> result;
	result.range = new Implemenatation::JoinRange<T, U, FLeftKey, FRightKey, FResult, NewT, FKeyComp>(
		Implemenatation::CloneRange(range), 
		Implemenatation::CloneRange(other.range), 
		leftKey,
		rightKey,
		resultSelector,
		keyComp
		);
	return result;
}

template<typename T>
template<typename U, typename FLeftKey, typename FRightKey, typename FResult, typename FKeyComp>
auto Linq<T>::join(
	Linq<U> other, 
	FLeftKey leftKey, 
	FRightKey rightKey, 
	FResult resultSelector, 
	FKeyComp keyComp
	) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
{
	typedef decltype(resultSelector(MakeType<T>(), MakeType<U>())) NewT;

	Linq<NewT> result;
	result.range = new Implemenatation::JoinRange<T, U, FLeftKey, FRightKey, FResult, NewT, FKeyComp>(
		Implemenatation::CloneRange(range), 
		Implemenatation::CloneRange(other.range), 
		leftKey,
		rightKey,
		resultSelector,
		keyComp
		);
	return result;
}

template<typename T>
template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
auto Linq<T>::leftJoin(
	Linq<U> other, 
	FLeftKey leftKey, 
	FRightKey rightKey, 
	FResult resultSelector,
    U defaultValue
    ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
{
    typedef std::decay<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>::type NewT;
	typedef decltype(leftKey(MakeType<T>())) TLeftKey;
	typedef decltype(rightKey(MakeType<U>())) TRightKey;

	auto keyComp = 
		[]
		(const TLeftKey &lh, const TRightKey &rh) -> bool
		{
			return lh == rh;
		};

	typedef decltype(keyComp) FKeyComp;
    
	Linq<NewT> result;
    
	result.range = new Implemenatation::LeftJoinRange<T, U, FLeftKey, FRightKey, FResult, NewT, FKeyComp>(
		Implemenatation::CloneRange(range), 
		Implemenatation::CloneRange(other.range), 
		leftKey,
		rightKey,
		resultSelector,
		keyComp,
        defaultValue
		);
        
	return result;
}

template<typename T>
template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
auto Linq<T>::fullJoin(
	Linq<U> other, 
	FLeftKey leftKey, 
	FRightKey rightKey, 
	FResult resultSelector,
    T defaultLValue,
    U defaultRValue
    ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
{
    typedef std::decay<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>::type NewT;
	typedef decltype(leftKey(MakeType<T>())) TLeftKey;
	typedef decltype(rightKey(MakeType<U>())) TRightKey;

	auto keyComp = 
		[]
		(const TLeftKey &lh, const TRightKey &rh) -> bool
		{
			return lh == rh;
		};

	typedef decltype(keyComp) FKeyComp;
    
	Linq<NewT> result;
    
	result.range = new Implemenatation::FullJoinRange<T, U, FLeftKey, FRightKey, FResult, NewT, FKeyComp>(
		Implemenatation::CloneRange(range), 
		Implemenatation::CloneRange(other.range), 
		leftKey,
		rightKey,
		resultSelector,
		keyComp,
        defaultLValue,
        defaultRValue
		);
        
	return result;
}


template<typename T>
Linq<typename T> Linq<T>::skip(int n)
{
	Linq<T> result;
	result.range = new Implemenatation::SkipRange<T>(Implemenatation::CloneRange(range), n);
	return result;
}

template<typename T>
template<typename F>
Linq<typename T> Linq<T>::skip(F f)
{
	Linq<T> result;
	result.range = new Implemenatation::SkipWhileRange<T, F>(Implemenatation::CloneRange(range), f);
	return result;
}


template<typename T>
Linq<typename T> Linq<T>::take(unsigned int n)
{
	Linq<T> result;
	result.range = new TakeRange<T>(CloneRange(range), n);
	return result;
}



template<typename T>
template<typename F>
auto Linq<T>::selectMany(F f) -> Linq< decltype(f(MakeType<T>()).value()) >
{
	typedef decltype(f(MakeType<T>()).value()) NewT;
	Linq<NewT> result;
	result.range = new SelectManyRange<NewT, T, F>(CloneRange(range), f);
	return result;
}

template<typename T>
template<typename F, typename G>
auto Linq<T>::groupBy(F f, G g) -> Linq<
	std::pair<
		typename std::remove_const<decltype(f(MakeType<T>()))>::type, 
		std::vector<typename std::remove_const<decltype(g(MakeType<T>()))>::type>
		>
	>
{
	typedef typename std::remove_const<decltype(f(MakeType<T>()))>::type TKey;
	typedef typename std::remove_const<decltype(g(MakeType<T>()))>::type TValue;
	typedef	std::pair<TKey, std::vector<typename TValue> > NewT;

	Linq<NewT> result;
	result.range = new GroupByRange<NewT, T, F, G, TKey, TValue>(CloneRange(range), f, g);
	return result;
}

template<typename T>
template<typename F>
auto Linq<T>::groupBy(F f) -> Linq<
	std::pair<
		typename std::remove_const<decltype(f(MakeType<T>()))>::type, 
		std::vector<T>
		>
	>
{
	return groupBy(f, [](const T &a){ return a;});
}

template<typename T>
T Linq<T>::first()
{
	range->rewind();
	if (range->empty())
		throw new InvalidOperationException();
	return range->front();
}

template<typename T>
T Linq<T>::firstOrDefault()
{
	range->rewind();
	if (range->empty())
		return T();
	return range->front();
}

template<typename T>
T Linq<T>::single()
{
	range->rewind();
	if (range->empty())
		throw new InvalidOperationException();
	auto result = range->popFront();
	if (!range->empty())
		throw new InvalidOperationException();
	return result;
}

template<typename T>
template<typename F>
T Linq<T>::elect(F f)
{
	range->rewind();
	if (range->empty())
		throw new InvalidOperationException();
	T* result = &range->popFront();
	while(!range->empty())
	{
		T *candidate = &range->popFront();
		if (f(*result, *candidate))
			continue;
 		result = candidate;
	}
	return *result;
}

template<typename T>
T Linq<T>::min()
{
	return elect(
		[]
		(decltype(const_reference()) lh, decltype(const_reference()) rh) 
		{ return lh < rh; }
		);
}

template<typename T>
T Linq<T>::max()
{
	return elect(
		[]
		(decltype(const_reference()) lh, decltype(const_reference()) rh) 
		{ return lh > rh; }
		);
}

template<typename T>
std::vector<typename T> Linq<T>::toVector()
{
	range->rewind();
	std::vector<typename T> result;
	while(!range->empty())
	{
		result.push_back(range->popFront());
	}
	return result;
}

template<typename T>
std::set<typename T> Linq<T>::toSet()
{
	range->rewind();
	std::set<typename T> result;
	while(!range->empty())
	{
		result.insert(range->popFront());
	}
	return result;
}

template<typename T>
std::list<typename T> Linq<T>::toList()
{
	range->rewind();
	std::list<typename T> result;
	while(!range->empty())
	{
		result.push_back(range->popFront());
	}
	return result;
}

template<typename T>
template<typename FKey, typename FValue>
auto Linq<T>::toMap(FKey key, FValue value) -> std::map<decltype(key(MakeType<T>())), decltype(value(MakeType<T>()))>
{
	typedef decltype(toMap(key,value)) TResult;
	TResult result;
	range->rewind();
	while(!range->empty())
	{
		T &v = range->popFront();
		result.insert(TResult::value_type(key(v), value(v)));
	}
	return result;
}

//=============================================================================
template<typename T>
LinqOrd<T>::LinqOrd()
{
}

template<typename T>
LinqOrd<T>::LinqOrd(const LinqOrd &rh)
	:Linq(rh)
{
}

template<typename T>
LinqOrd<T>::LinqOrd(LinqOrd &&rh)
	:Linq(rh)
{
}

template<typename T>
typename LinqOrd<T>& LinqOrd<T>::operator=(const LinqOrd &rh)
{
	Linq &lh = *this;
	lh = rh;
	return *this;
}

template<typename T>
typename LinqOrd<T>& LinqOrd<T>::operator=(LinqOrd<T> &&rh)
{
	Linq &lh = *this;
	lh = rh;
	return *this;
}
//-----------------------------------------------------------------------------
template<typename T>
template<typename U>
LinqOrd<typename T> LinqOrd<T>::thenBy(U f)
{

	typedef decltype(f(MakeType<T>())) TKey;
	auto comp = [](const TKey &lh, const TKey &rh) { return lh < rh ;};
	return thenBy(f, comp);
}

template<typename T>
template <typename U, typename Comp>
LinqOrd<typename T> LinqOrd<T>::thenBy(U f, Comp comp)
{
	LinqOrd<typename T> result;
	auto orderBy = dynamic_cast<Implemenatation::BaseOrderByRange<T> *>(Implemenatation::CloneRange(range));
	orderBy->addComparator(f, comp, false);
	result.range = orderBy;
	return result;
}

template<typename T>
template <typename U, typename Comp>
LinqOrd<typename T> LinqOrd<T>::thenByDesc(U f, Comp comp)
{
	LinqOrd<typename T> result;
	auto orderBy = dynamic_cast<Implemenatation::BaseOrderByRange<T> *>(Implemenatation::CloneRange(range));
	orderBy->addComparator(f, comp, true);
	result.range = orderBy;
	return result;
}

template<typename T>
template<typename U>
LinqOrd<typename T> LinqOrd<T>::thenByDesc(U f)
{
	typedef decltype(f(MakeType<T>())) TKey;
	auto comp = [](const TKey &lh, const TKey &rh) { return lh < rh ;};
	return thenByDesc(f, comp);
}

//=============================================================================
template <typename TCont>
auto from(TCont &src) -> Linq<typename std::remove_reference<decltype(*(src.begin()))>::type>
{
	typedef std::remove_reference<decltype(*(src.begin()))>::type T;
	Linq<typename T> result;
	result.range = new Implemenatation::ContainerRange<TCont, T>(src);
	return result;
}

template <typename TIter>
auto from(TIter b, TIter e) -> Linq<typename std::iterator_traits<TIter>::value_type>
{
	typedef std::iterator_traits<TIter>::value_type T;
	Linq<typename T> result;
	result.range = new Implemenatation::IteratorRange<TIter>(b, e);
	return result;
}
//=============================================================================

}