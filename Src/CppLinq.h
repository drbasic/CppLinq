#pragma once

#include <array>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <functional>
#include <type_traits>
int RangeCount = 0;

namespace Linq{
    class InvalidOperationException{};
    template<typename T>
    class Linq;

    template<typename T>
    class LinqOrd;

    template<typename T>
    T MakeType();

    template<typename T>
    class Range
    {
    public:
        Range(){ RangeCount++; }
        virtual ~Range() { RangeCount--; }
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

        typedef typename std::remove_const<T>::type CleanT;
        //-------------------------------------------------------------------------
        Linq<CleanT> removeConst();

        template<typename F>
        T aggregate(F f);
        template<typename TAccumulate, typename F>
        TAccumulate aggregate(TAccumulate seed, F f);
        template<typename TAccumulate, typename F, typename R>
        auto aggregate(TAccumulate seed, F f, R r) -> decltype(r(MakeType<TAccumulate>()));

        template<typename F>
        bool all(F f);

        template<typename F>
        bool any(F f);
        bool any();

        Linq<T> concat(const Linq<T> &second);

        template<typename F>
        bool contains(T val, F f);
        bool contains(T val);

        template<typename F>
        size_t count(F f);
        size_t count();

        Linq<T> defaultIfEmpty();
        Linq<T> defaultIfEmpty(const T &defaultValue);

        template<typename F>
        Linq<T> distinct(F f);
        Linq<T> distinct();

        T elementAt(unsigned int index);
        T elementAtOrDefault(unsigned int index);
        T elementAtOrDefault(unsigned int index, const T &defaultValue);

        template <typename U, typename TComp>
        Linq<T> except(Linq<U> other, TComp comp);
        Linq<T> except(Linq<T> other);

        template <typename U>
        Linq<T> where(U f);

        LinqOrd<T> orderBy();
        template <typename U>
        LinqOrd<T> orderBy(U f);
        template <typename U, typename Comp>
        LinqOrd<T> orderBy(U f, Comp comp);

        LinqOrd<T> orderByDesc();
        template <typename U>
        LinqOrd<T> orderByDesc(U f);
        template <typename U, typename Comp>
        LinqOrd<T> orderByDesc(U f, Comp comp);

        template <typename U>
        auto select(U f) -> Linq<decltype(f(MakeType<T>()))>;

        Linq<T> skip(int n);

        template <typename F>
        Linq<T> skipWhile(F f);

        Linq<T> take(unsigned int n);

        template <typename F>
        Linq<T> takeWhile(F f);

        template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
        auto join(
            Linq<U> other,
            FLeftKey leftKey,
            FRightKey rightKey,
            FResult resultSelector
            ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;

        template<typename U, typename FLeftKey, typename FRightKey, typename FResult, typename FLessComp>
        auto join(
            Linq<U> other,
            FLeftKey leftKey,
            FRightKey rightKey,
            FResult resultSelector,
            FLessComp lessComp
            ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;

        template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
        auto leftJoin(
            Linq<U> other,
            FLeftKey leftKey,
            FRightKey rightKey,
            FResult resultSelector
            ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;

        template<typename U, typename FLeftKey, typename FRightKey, typename FResult, typename FKeyComp>
        auto leftJoin(
            Linq<U> other,
            FLeftKey leftKey,
            FRightKey rightKey,
            FResult resultSelector,
            FKeyComp keyComp
            ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;

        template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
        auto leftJoinWithDefault(
            Linq<U> other,
            FLeftKey leftKey,
            FRightKey rightKey,
            FResult resultSelector,
            U defaultValue
            ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>;

        template<typename U, typename FLeftKey, typename FRightKey, typename FResult, typename FKeyComp>
        auto leftJoinWithDefault(
            Linq<U> other,
            FLeftKey leftKey,
            FRightKey rightKey,
            FResult resultSelector,
            FKeyComp keyComp,
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

        Linq<T> reverse();

        T first();
        T firstOrDefault();
        T firstOrDefault(const T &defaultValue);

        T single();
        T singleOrDefault();
        T singleOrDefault(const T &defaultValue);

        T last();
        T lastOrDefault();
        T lastOrDefault(const T &defaultValue);

        bool empty();

        template<typename F>
        T elect(F f);

        T min();

        T max();
        
        std::pair<T, T> minMax();

        T sum();

        template<typename F>
        auto sum(F f) -> decltype(f(MakeType<T>()));

        T avg();

        template<typename F>
        auto avg(F f) -> decltype(f(MakeType<T>()));


        std::vector<T> toVector();

        std::set<T> toSet();

        std::list<T> toList();

        template<typename FKey, typename FValue>
        auto toMap(FKey key, FValue value) -> std::map<decltype(key(MakeType<T>())), decltype(value(MakeType<T>()))>;

#ifdef QVECTOR_H
        QVector<T> toQVector();
#endif

    public:
        Range<T> *range;
    };
    //=============================================================================
    template<typename T>
    class LinqOrd: public Linq<T>
    {
    public:
        LinqOrd();
        LinqOrd(const LinqOrd &rh);
        LinqOrd(LinqOrd &&rh);
        LinqOrd& operator=(const LinqOrd &rh);
        LinqOrd& operator=(LinqOrd &&rh);

        template <typename U>
        LinqOrd<T> thenBy(U f);
        template <typename U, typename Comp>
        LinqOrd<T> thenBy(U f, Comp comp);
        template <typename U>
        LinqOrd<T> thenByDesc(U f);
        template <typename U, typename Comp>
        LinqOrd<T> thenByDesc(U f, Comp comp);
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
                return *(iter_++);
            }

            T& front() override
            {
                if (empty())
                    throw new InvalidOperationException();
                return *iter_;
            }

            void rewind() override
            {
                iter_ = b_;
            }

            Range<T>* clone() override
            {
                auto result = new IteratorRange(b_, e_);
                return result;
            }
        private:
            TIter b_;
            TIter e_;
            TIter iter_;
        };

        template<typename T>
        class ArrayRange : public Range<T>
        {
        public:
            typedef T* TIter;
            ArrayRange(T *b, T *e)
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
                return *(iter_++);
            }

            T& front() override
            {
                if (empty())
                    throw new InvalidOperationException();
                return *iter_;
            }

            void rewind() override
            {
                iter_ = b_;
            }

            Range<T>* clone() override
            {
                auto result = new ArrayRange(b_, e_);
                return result;
            }
        private:
            TIter b_;
            TIter e_;
            TIter iter_;
        };

        template<typename TCont, typename TIter, typename TValue>
        class ContainerRange : public Range<TValue>
        {
        public:
            typedef TValue T;

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

            Range<T>* clone() override
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
        class RepeatRange : public Range<T>
        {
        public:
            RepeatRange(T value, unsigned int count)
                : value_(value)
                , count_(count)
                , index_(0)
            {
            }

            bool empty() override
            {
                return (index_ >= count_);
            }

            T& popFront() override
            {
                if (empty())
                    throw new InvalidOperationException();
                index_++;
                return value_;
            }

            T& front() override
            {
                if (empty())
                    throw new InvalidOperationException();
                return value_;
            }

            void rewind() override
            {
                index_ = 0;
            }

            Range<T>* clone() override
            {
                auto result = new RepeatRange(value_, count_);
                return result;
            }
        private:
            T value_;
            unsigned int count_;
            unsigned int index_;
        };

        template<typename T>
        class EmptyRange : public Range<T>
        {
        public:
            EmptyRange()
            {
            }

            bool empty() override
            {
                return true;
            }

            T& popFront() override
            {
                throw new InvalidOperationException();
            }

            T& front() override
            {
                throw new InvalidOperationException();
            }

            void rewind() override
            {
            }

            Range<T>* clone() override
            {
                auto result = new EmptyRange<T>();
                return result;
            }
        };

        template<typename T, typename U, typename TComp>
        class ExceptRange : public Range<T>
        {
        public:
            ExceptRange(Range<T> *first, Range<U> *second, TComp comp)
                : first_(first)
                , second_(second)
                , comp_(comp)
            {
            }

            ~ExceptRange()
            {
                delete first_;
                delete second_;
            }

            bool empty() override
            {
                if (!frontReady)
                    seekFront();
                return first_->empty();
            }

            T& popFront() override
            {
                if (!frontReady)
                    seekFront();
                auto &result = first_->front();
                frontReady = false;
                first_->popFront();
                seekFront();
                return result;
            }

            T& front() override
            {
                return first_->front();
            }

            void rewind() override
            {
                frontReady = false;
                first_->rewind();
            }

            Range<T>* clone() override
            {
                auto result = new ExceptRange<T, U, TComp>(CloneRange(first_), CloneRange(second_), comp_);
                return result;
            }
        private:
            Range<T> *first_;
            Range<U> *second_;
            TComp comp_;
            bool frontReady;

            void seekFront()
            {
                while(!first_->empty())
                {
                    T &tmp = first_->front();

                    bool matchFound = false;
                    second_->rewind();
                    while(!second_->empty())
                    {
                        if (comp_(tmp, second_->popFront()))
                        {
                            matchFound = true;
                            break;
                        }
                    }

                    if(!matchFound)
                        break;
                    //переходим к следующему элементу, т.к. нашли совпадение во втором списке
                    first_->popFront();
                }
                frontReady = true;
            }
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

            Range<T>* clone() override
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

            Range<T>* clone() override
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

            Range<T>* clone() override
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
                this->addComparator(f, comp, isDesc);
            }

            ~OrderByRange()
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
                if (!prepared_)
                    prepare();
                if (iter_ == end_)
                    throw new InvalidOperationException();
                return **(iter_++);
            }

            T& front() override
            {
                if (!prepared_)
                    prepare();
                if (iter_ == end_)
                    throw new InvalidOperationException();
                return **(iter_);
            }

            void rewind() override
            {
                src_->rewind();
                prepared_ = false;
                orderedData_.clear();
            }

            Range<T>* clone() override
            {
                auto result = new OrderByRange(CloneRange(src_), f_, comp_, isDesc_);
                result->comparators = this->comparators;
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
            typename std::vector< T*>::iterator end_;

            void prepare()
            {
                while(!src_->empty())
                    orderedData_.push_back(&src_->popFront());

                if (this->comparators.size() == 1)
                {
                    
                if (!isDesc_)
                        std::sort(
                        orderedData_.begin(),
                        orderedData_.end(),
                        [=](const T *lh, const T *rh) -> bool{ return comp_(f_(*lh), f_(*rh)) ;}
                        );
                    else
                        std::sort(
                        orderedData_.begin(),
                        orderedData_.end(),
                        [=](const T *lh, const T *rh) -> bool{ return comp_(f_(*rh), f_(*lh)) ;}
                        );
                }
                else
                {
                    auto aggComp =
                        [&]
                    (const T *lh, const T *rh) -> bool
                    {
                        for (auto ii = this->comparators.begin(); ii != this->comparators.end(); ++ii)
                        {
                            int c = (*ii)(lh, rh);
                            if (c != 0)
                                return c < 0;
                        }
                        return false;
                    };
                    std::sort(
                       orderedData_.begin(),
                       orderedData_.end(),
                       aggComp
                    );
                }


                iter_ = orderedData_.begin();
                end_ = orderedData_.end();
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

            Range<T>* clone() override
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

            Range<T>* clone() override
            {
                auto result = new SkipRange<T>(CloneRange(src_), n_);
                return result;
            }
        private:
            Range<T> *src_;
            unsigned int n_;
            bool prepared_;

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

            Range<T>* clone() override
            {
                auto result = new SkipWhileRange<T, F>(CloneRange(src_), f_);
                return result;
            }
        private:
            Range<T> *src_;
            F f_;
            bool prepared_;

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
                , taken_(0)
                , n_(n)
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

            Range<T>* clone() override
            {
                auto result = new TakeRange<T>(CloneRange(src_), n_);
                return result;
            }
        private:
            Range<T> *src_;
            unsigned int taken_;
            unsigned int n_;
        };

        template<typename T, typename F>
        class TakeWhileRange : public Range<T>
        {
        public:
            TakeWhileRange(Range<T> *src, F f)
                : src_(src)
                , f_(f)
            {
            }

            ~TakeWhileRange()
            {
                delete src_;
            }

            bool empty() override
            {
                return src_->empty() || !f_(src_->front());
            }

            T& popFront() override
            {
                if (empty())
                    throw new InvalidOperationException();
                return src_->popFront();
            }

            T& front() override
            {
                return src_->front();
            }

            void rewind() override
            {
                src_->rewind();
            }

            Range<T>* clone() override
            {
                auto result = new TakeWhileRange<T, F>(CloneRange(src_), f_);
                return result;
            }
        private:
            Range<T> *src_;
            F f_;
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

        template<typename LT, typename RT, typename FLeftKey, typename FRightKey, typename FResult, typename TResult, typename FLessComp>
        class JoinRange : public BaseInnerJoinRange<LT, RT, TResult>
        {
        public:
            JoinRange(Range<LT> *srcL, Range<RT> *srcR, FLeftKey leftKey, FRightKey rightKey, FResult selectResult, FLessComp lessComp)
                : BaseInnerJoinRange<LT, RT, TResult>(srcL, srcR)
                , leftKey_(leftKey)
                , rightKey_(rightKey)
                , selectResult_(selectResult)
                , lessComp_(lessComp)
            {
            }

            Range<TResult>* clone() override
            {
                auto result = new JoinRange<LT, RT, FLeftKey, FRightKey, FResult, TResult, FLessComp>(
                    CloneRange(this->srcL_),
                    CloneRange(this->srcR_),
                    leftKey_,
                    rightKey_,
                    selectResult_,
                    lessComp_
                    );
                return result;
            }
        protected:
            void prepare() override
            {
                this->prepared_ = true;
                if (this->srcL_->empty() || this->srcR_->empty())
                    return;

                typedef decltype(leftKey_(MakeType<LT>())) LeftKey;
                typedef decltype(rightKey_(MakeType<RT>())) RightKey;
                //подготавливаем список ключ-значение из левого списка
                typedef std::pair<LeftKey, LT*> LeftKeyValue;
                std::vector<LeftKeyValue> lhData;
                while(!this->srcL_->empty())
                {
                    auto pVal = &this->srcL_->popFront();
                    lhData.push_back(LeftKeyValue(leftKey_(*pVal), pVal));
                }
                std::sort(
                    lhData.begin(), 
                    lhData.end(), 
                    [=]
                    (const LeftKeyValue& a, const LeftKeyValue& b)
                    { return lessComp_(a.first, b.first);}
                    );

                //подготавливаем список ключ-значение из правого списка
                typedef std::pair<RightKey, RT*> RightKeyValue;
                std::vector<RightKeyValue> rhData;
                while(!this->srcR_->empty())
                {
                    auto pVal = &this->srcR_->popFront();
                    rhData.push_back(RightKeyValue(rightKey_(*pVal), pVal));
                }
                std::sort(
                    rhData.begin(), 
                    rhData.end(), 
                    [=]
                    (const RightKeyValue& a, const RightKeyValue& b)
                    { return lessComp_(a.first, b.first);}
                    );

                //проходим по спискам и выбираем эквивалентные значения
                auto li = lhData.begin();
                auto le = lhData.end();
                auto ri = rhData.begin();
                auto re = rhData.end();
                while(li != le && ri != re)
                {
                    bool leftLessRight = lessComp_(li->first, ri->first);
                    bool rightLessLeft = lessComp_(ri->first, li->first);
                    if (!leftLessRight && !rightLessLeft)
                    {
                        this->data_.push_back(selectResult_(*li->second, *ri->second));
                        ++li;
                        ++ri;
                    }
                    else if (leftLessRight)
                    {
                        ++li;
                    }
                    else if (rightLessLeft)
                    {
                        ++ri;
                    }
                    else
                    {
                        ++li;
                        ++ri;
                    }
                }
                this->iter_ = this->data_.begin();
            }
        private:
            FLeftKey leftKey_;
            FRightKey rightKey_;
            FResult selectResult_;
            FLessComp lessComp_;
        };

        template<typename LT, typename RT, typename FLeftKey, typename FRightKey, typename FResult, typename TResult, typename FKeyComp>
        class LeftJoinRange : public BaseInnerJoinRange<LT, RT, TResult>
        {
        public:
            LeftJoinRange(Range<LT> *srcL, Range<RT> *srcR, FLeftKey leftKey, FRightKey rightKey, FResult selectResult, FKeyComp keyComp, RT defaultValue)
                : BaseInnerJoinRange<LT, RT, TResult>(srcL, srcR)
                , leftKey_(leftKey)
                , rightKey_(rightKey)
                , selectResult_(selectResult)
                , keyComp_(keyComp)
                , defaultValue_(defaultValue)
            {
            }

            Range<TResult>* clone() override
            {
                auto result = new LeftJoinRange<LT, RT, FLeftKey, FRightKey, FResult, TResult, FKeyComp>(
                    CloneRange(this->srcL_),
                    CloneRange(this->srcR_),
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
                this->prepared_ = true;
                if (this->srcL_->empty())
                    return;

                //подготавливаем список ключ-значение из правого списка
                typedef std::pair<decltype(rightKey_(MakeType<RT>())), RT*> KeyValue;
                std::vector<KeyValue> rhData;
                while(!this->srcR_->empty())
                {
                    auto pVal = &this->srcR_->popFront();
                    rhData.push_back(KeyValue(rightKey_(*pVal), pVal));
                }

                //двигаемся по левому списку
                while(!this->srcL_->empty())
                {
                    //вычилсяем значение
                    LT *lh = &this->srcL_->popFront();
                    //и ключ
                    auto lhKey = leftKey_(*lh);

                    bool isFound = false;
                    //теперь ищем в правом списке такие же ключи
                    for(auto ii = rhData.begin() ; ii != rhData.end(); ++ii)
                    {
                        //сравниваем ключи на эквивалентность
                        if (keyComp_(lhKey, ii->first))
                        {
                            this->data_.push_back(selectResult_(*lh, *(ii->second)));
                            isFound = true;
                        }
                    }

                    if (!isFound)
                        this->data_.push_back(selectResult_(*lh, defaultValue_));
                }
                this->iter_ = this->data_.begin();
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
                : BaseInnerJoinRange<LT, RT, TResult>(srcL, srcR)
                , leftKey_(leftKey)
                , rightKey_(rightKey)
                , selectResult_(selectResult)
                , keyComp_(keyComp)
                , defaultLValue_(defaultLValue)
                , defaultRValue_(defaultRValue)
            {
            }

            Range<TResult>* clone() override
            {
                auto result = new FullJoinRange<LT, RT, FLeftKey, FRightKey, FResult, TResult, FKeyComp>(
                    CloneRange(this->srcL_),
                    CloneRange(this->srcR_),
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
                this->prepared_ = true;

                //подготавливаем список ключ-значение из правого списка
                typedef decltype(rightKey_(MakeType<RT>())) TRKey;
                typedef std::pair<TRKey, RT*> RKeyValue;
                std::vector<RKeyValue> rhData;
                while(!this->srcR_->empty())
                {
                    auto pVal = &this->srcR_->popFront();
                    rhData.push_back(RKeyValue(rightKey_(*pVal), pVal));
                }

                std::set<TRKey> usedRKeys;
                //двигаемся по левому списку
                while(!this->srcL_->empty())
                {
                    //вычилсяем значение
                    LT *lh = &this->srcL_->popFront();
                    //и ключ
                    auto lhKey = leftKey_(*lh);

                    bool isFound = false;
                    //теперь ищем в правом списке такие же ключи
                    for(auto ii = rhData.begin() ; ii != rhData.end(); ++ii)
                    {
                        //сравниваем ключи на эквивалентность
                        if (keyComp_(lhKey, ii->first))
                        {
                            this->data_->push_back(selectResult_(*lh, *(ii->second)));
                            isFound = true;
                            usedRKeys.insert(ii->first);
                        }
                    }

                    if (!isFound)
                        this->data_->push_back(selectResult_(*lh, defaultRValue_));
                }

                auto endIter = usedRKeys.end();
                for (auto ii = rhData.begin(), eii = rhData.end(); ii != eii; ++ii)
                {
                    if (usedRKeys.find(ii->first) == endIter)
                        //если не нашли ключ среди использованных, то добавляем правый объект
                        this->data_->push_back(selectResult_(defaultLValue_, *(ii->second)));
                }

                this->iter_ = this->data_->begin();
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
                return iter_ == end_;
            }

            T& popFront() override
            {
                if (!prepared_)
                    prepare();
                if (iter_ == end_)
                    throw new InvalidOperationException();
                return *(iter_++);
            }

            T& front() override
            {
                if (!prepared_)
                    prepare();
                if (iter_ == end_)
                    throw new InvalidOperationException();
                return *(iter_);
            }

            void rewind() override
            {
                src_->rewind();
                prepared_ = false;
                data_.clear();
            }

            Range<T>* clone() override
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
            std::vector<T> data_;
            typename std::vector<T>::iterator iter_;
            typename std::vector<T>::iterator end_;

            void prepare()
            {
                prepared_ = true;
                while(!src_->empty())
                {
                    auto inner = f_(src_->front());
                    while(!inner.range->empty())
                    {
                        data_.push_back(inner.range->popFront());
                    }
                    //переходим к следующему элементу
                    src_->popFront();
                }
                iter_ = data_.begin();
                end_ = data_.end();
            }
        };

        template<typename T, typename U, typename F>
        class SelectManyRange2 : public Range<T>
        {
        public:
            SelectManyRange2(Range<U> *src, F f)
                : src_(src)
                , f_(f)
                , prepared_(false)
            {
            }

            ~SelectManyRange2()
            {
                delete src_;
            }

            bool empty() override
            {
                if (!prepared_)
                    prepare();
                return src_->empty() || currentInner_->range->empty();
            }

            T& popFront() override
            {
                T& result = currentInner_->range->front();
                findNext();
                return result;
            }

            T& front() override
            {
                return currentInner_->range->front();
            }

            void rewind() override
            {
                src_->rewind();
                prepared_ = false;
            }

            Range<T>* clone() override
            {
                auto result = new SelectManyRange2<T, U, F>(
                    CloneRange(src_),
                    f_
                    );
                return result;
            }
        private:
            Range<U> *src_;
            bool prepared_;
            F f_;
            std::list<Linq<T>> innerRanges_;
            Linq<T> *currentInner_;

            void prepare()
            {
                prepared_ = true;
                innerRanges_.push_back(Linq<T>());
                currentInner_ = &(innerRanges_.front());
                while(!src_->empty())
                {
                    //получаем внутреннее множество
                    *currentInner_ = f_(src_->front());
                    currentInner_->range->rewind();
                    //если оно не пустое, значит нашли начало последовательности
                    if (!currentInner_->range->empty())
                        return;
                    //переходим к следующему элементу
                    src_->popFront();
                }
            }

            bool advanceInner()
            {
                if (currentInner_->range->empty())
                    return false;
                currentInner_->range->popFront();
                return !currentInner_->range->empty();
            }

            void findNext()
            {
                Linq<T> *nextInner = nullptr;
                while(!src_->empty())
                {
                    if (advanceInner())
                        return;
                    src_->popFront();
                    if (src_->empty())
                        return;
                    //получаем следующее внутреннее множество
                    if (nextInner == nullptr)
                    {
                        innerRanges_.push_back(Linq<T>());
                        nextInner = &(innerRanges_.back());
                    }
                    *nextInner = f_(src_->front());
                    nextInner->range->rewind();
                    if (!nextInner->range->empty())
                    {
                        currentInner_ = nextInner;
                        return;
                    }
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
            {
            }

            ~DistinctRange()
            {
                delete src_;
            }

            bool empty() override
            {
                return src_->empty();
            }

            T& popFront() override
            {
                T& result = front();
                for(src_->popFront();!src_->empty();src_->popFront())
                {
                    T& next = src_->front();
                    if (f_(result) != f_(next))
                        break;
                }
                return result;
            }

            T& front() override
            {
                return src_->front();
            }

            void rewind() override
            {
                src_->rewind();
            }

            Range<T>* clone() override
            {
                auto result = new DistinctRange<T, F, TKey>(
                    CloneRange(src_),
                    f_
                    );
                return result;
            }
        private:
            Range<T> *src_;
            F f_;
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

            Range<T>* clone() override
            {
                auto result = new GroupByRange<T, TSrc, F, G, TKey, TValue>(
                    CloneRange(src_),
                    f_,
                    g_
                    );
                return result;
            }
        private:
            Range<TSrc> *src_;
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
                typedef std::vector<TValue> TValues;
                typename std::map<TKey, TValues> tmp;

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

        template<typename T>
        class ReverseRange : public Range<T>
        {
        public:
            ReverseRange(Range<T> *src)
                : src_(src)
                , prepared_(false)
            {
            }

            ~ReverseRange()
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
                if (!prepared_)
                    prepare();
                if (iter_ == end_)
                    throw new InvalidOperationException();
                return **(iter_++);
            }

            T& front() override
            {
                if (!prepared_)
                    prepare();
                if (iter_ == end_)
                    throw new InvalidOperationException();
                return **(iter_);
            }

            void rewind() override
            {
                src_->rewind();
                prepared_ = false;
                orderedData_.clear();
            }

            Range<T>* clone() override
            {
                auto result = new ReverseRange<T>(CloneRange(src_));
                return result;
            }

            Range<T>* srcRange()
            {
                return src_;
            }
        private:
            Range<T> *src_;
            bool prepared_;
            typename std::vector<T*> orderedData_;
            typename std::vector<T*>::reverse_iterator iter_;
            typename std::vector<T*>::reverse_iterator end_;

            void prepare()
            {
                orderedData_.clear();
                while(!src_->empty())
                {
                    orderedData_.push_back(&src_->popFront());
                }
                prepared_ = true;
                iter_ = orderedData_.rbegin();
                end_ = orderedData_.rend();
            }
        };

        template<typename T, typename SrcT>
        class RemoveConstRange : public Range<T>
        {
        public:
            RemoveConstRange(Range<SrcT> *src)
                : src_(src)
            {
            }

            ~RemoveConstRange()
            {
                delete src_;
            }

            bool empty() override
            {
                return src_->empty();
            }

            T& popFront() override
            {
                return (T&)src_->popFront();
            }

            T& front() override
            {
                return (T&)src_->front();
            }

            void rewind() override
            {
                src_->rewind();
            }

            Range<T>* clone() override
            {
                auto result = new RemoveConstRange<T, SrcT>(CloneRange(src_));
                return result;
            }
        private:
            Range<SrcT> *src_;
        };
    }//Implemenatation
    //=============================================================================


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
    Linq<T>& Linq<T>::operator=(const Linq &rh)
    {
        if (this == &rh)
            return *this;
        delete range;
        range = Implemenatation::CloneRange(rh.range);
        return *this;
    }

    template<typename T>
    Linq<T>& Linq<T>::operator=(Linq &&rh)
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
    T Linq<T>::aggregate(F f)
    {
        return aggregate(
            T(),
            f,
            [](const T &a){ return a; }
        );
    }

    template<typename T>
    template<typename TAccumulate, typename F>
    TAccumulate Linq<T>::aggregate(TAccumulate seed, F f)
    {
        return aggregate(
            seed,
            f,
            [](const TAccumulate &a){ return a; }
        );
    }

    template<typename T>
    template<typename TAccumulate, typename F, typename R>
    auto Linq<T>::aggregate(TAccumulate seed, F f, R r) -> decltype(r(MakeType<TAccumulate>()))
    {
        typedef decltype(r(MakeType<TAccumulate>())) TResult;

        range->rewind();
        while(!range->empty())
        {
            seed = f(seed, range->popFront());
        }
        return r(seed);
    }

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
    bool Linq<T>::any()
    {
        range->rewind();
        return !range->empty();
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
    Linq<T> Linq<T>::concat(const Linq<T> &second)
    {
        Linq<T> result;
        result.range = new Implemenatation::ConcatRange<T>(Implemenatation::CloneRange(range), Implemenatation::CloneRange(second.range));
        return result;
    }

    template<typename T>
    Linq<T> Linq<T>::defaultIfEmpty(const T &defaultValue)
    {
        Linq<T> result;
        result.range = new Implemenatation::DefaultIfEmptyRange<T>(Implemenatation::CloneRange(range), defaultValue);
        return result;
    }

    template<typename T>
    Linq<T> Linq<T>::defaultIfEmpty()
    {
        return defaultIfEmpty(T());
    }

    template<typename T>
    template<typename F>
    Linq<T> Linq<T>::distinct(F f)
    {
        Linq<T> result = orderBy(f);
        result.range = new Implemenatation::DistinctRange<T, F, decltype(f(MakeType<T>()))>(result.range, f);
        return result;
    }

    template<typename T>
    Linq<T> Linq<T>::distinct()
    {
        return distinct([](const T &a) { return a; });
    }

    template<typename T>
    T Linq<T>::elementAt(unsigned int index)
    {
        range->rewind();
        int i = 0;
        while(!range->empty())
        {
            if (index == i++)
                return range->front();
            range->popFront();
        }
        throw new InvalidOperationException();
    }

    template<typename T>
    T Linq<T>::elementAtOrDefault(unsigned int index)
    {
        return elementAtOrDefault(index, T());
    }

    template<typename T>
    T Linq<T>::elementAtOrDefault(unsigned int index, const T &defaultValue)
    {
        range->rewind();
        int i = 0;
        while(!range->empty())
        {
            if (index == i++)
                return range->front();
            range->popFront();
        }
        return defaultValue;
    }

    template<typename T>
    template <typename U, typename TComp>
    Linq<T> Linq<T>::except(Linq<U> other, TComp comp)
    {
        Linq<T> result;
        result.range = new Implemenatation::ExceptRange<T, U, TComp>(
            Implemenatation::CloneRange(range),
            Implemenatation::CloneRange(other.range),
            comp
            );
        return result;
    }

    template<typename T>
    Linq<T> Linq<T>::except(Linq<T> other)
    {
        auto comp = [](const T &lh, const T &rh){ return lh == rh; };
        return except(other, comp);
    }

    template<typename T>
    template<typename U>
    Linq<T> Linq<T>::where(U f)
    {
        Linq<T> result;
        result.range = new Implemenatation::WhereRange<T, U>(Implemenatation::CloneRange(range), f);
        return result;
    }

    template<typename T>
    LinqOrd<T> Linq<T>::orderBy()
    {
        auto f = [](const T &a) { return a; };
        auto comp = [](const T &lh, const T &rh) { return lh < rh ;};
        return orderBy(f, comp);
    }

    template<typename T>
    template<typename U>
    LinqOrd<T> Linq<T>::orderBy(U f)
    {
        typedef decltype(f(MakeType<T>())) TKey;
        auto comp = [](const TKey &lh, const TKey &rh) { return lh < rh ;};
        return orderBy(f, comp);
    }

    template<typename T>
    template<typename U, typename Comp>
    LinqOrd<T> Linq<T>::orderBy(U f, Comp comp)
    {
        LinqOrd<T> result;
        result.range = new Implemenatation::OrderByRange<T, U, Comp>(Implemenatation::CloneRange(range), f, comp, false);
        return result;
    }

    template<typename T>
    LinqOrd<T> Linq<T>::orderByDesc()
    {
        auto f = [](const T &a) { return a; };
        auto comp = [](const T &lh, const T &rh) { return lh < rh ;};
        return orderByDesc(f, comp);
    }

    template<typename T>
    template<typename U>
    LinqOrd<T> Linq<T>::orderByDesc(U f)
    {
        typedef decltype(f(MakeType<T>())) TKey;
        auto comp = [](const TKey &lh, const TKey &rh) { return lh < rh; };
        return orderByDesc(f, comp);
    }

    template<typename T>
    template<typename U, typename Comp>
    LinqOrd<T> Linq<T>::orderByDesc(U f, Comp comp)
    {
        typedef decltype(f(MakeType<T>())) TKey;
        LinqOrd<T> result;
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
        ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
    {
        typedef decltype(leftKey(MakeType<T>())) TLeftKey;
        typedef decltype(rightKey(MakeType<U>())) TRightKey;

        auto lessComp =
            []
            (const TLeftKey &lh, const TRightKey &rh) -> bool
            {
                return lh < rh;
            };

        return join(other, leftKey, rightKey, resultSelector, lessComp);
    }

    template<typename T>
    template<typename U, typename FLeftKey, typename FRightKey, typename FResult, typename FLessComp>
    auto Linq<T>::join(
        Linq<U> other,
        FLeftKey leftKey,
        FRightKey rightKey,
        FResult resultSelector,
        FLessComp lessComp
        ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
    {
        typedef decltype(resultSelector(MakeType<T>(), MakeType<U>())) NewT;

        Linq<NewT> result;
        result.range = new Implemenatation::JoinRange<T, U, FLeftKey, FRightKey, FResult, NewT, FLessComp>(
            Implemenatation::CloneRange(range),
            Implemenatation::CloneRange(other.range),
            leftKey,
            rightKey,
            resultSelector,
            lessComp
            );
        return result;
    }

    template<typename T>
    template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
    auto Linq<T>::leftJoin(
        Linq<U> other,
        FLeftKey leftKey,
        FRightKey rightKey,
        FResult resultSelector
        ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
    {
        typedef decltype(leftKey(MakeType<T>())) TLeftKey;
        typedef decltype(rightKey(MakeType<U>())) TRightKey;

        auto keyComp =
            []
            (const TLeftKey &lh, const TRightKey &rh) -> bool
            {
                return lh == rh;
            };

        return leftJoin(other, leftKey, rightKey, resultSelector, keyComp);
    }

    template<typename T>
    template<typename U, typename FLeftKey, typename FRightKey, typename FResult, typename FKeyComp>
    auto Linq<T>::leftJoin(
        Linq<U> other,
        FLeftKey leftKey,
        FRightKey rightKey,
        FResult resultSelector,
        FKeyComp keyComp
        ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
    {
        return leftJoinWithDefault(other, leftKey, rightKey, resultSelector, keyComp, U());
    }

    template<typename T>
    template<typename U, typename FLeftKey, typename FRightKey, typename FResult>
    auto Linq<T>::leftJoinWithDefault(
        Linq<U> other,
        FLeftKey leftKey,
        FRightKey rightKey,
        FResult resultSelector,
        U defaultValue
        ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
    {
        typedef decltype(leftKey(MakeType<T>())) TLeftKey;
        typedef decltype(rightKey(MakeType<U>())) TRightKey;

        auto keyComp =
            []
            (const TLeftKey &lh, const TRightKey &rh) -> bool
            {
                return lh == rh;
            };

        return leftJoinWithDefault(other, leftKey, rightKey, resultSelector, keyComp, defaultValue);
    }

    template<typename T>
    template<typename U, typename FLeftKey, typename FRightKey, typename FResult, typename FKeyComp>
    auto Linq<T>::leftJoinWithDefault(
        Linq<U> other,
        FLeftKey leftKey,
        FRightKey rightKey,
        FResult resultSelector,
        FKeyComp keyComp,
        U defaultValue
        ) -> Linq<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>
    {
        typedef typename std::decay<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>::type NewT;

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
        typedef typename std::decay<decltype(resultSelector(MakeType<T>(), MakeType<U>()))>::type NewT;
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
    Linq<T> Linq<T>::skip(int n)
    {
        Linq<T> result;
        result.range = new Implemenatation::SkipRange<T>(Implemenatation::CloneRange(range), n);
        return result;
    }

    template<typename T>
    template<typename F>
    Linq<T> Linq<T>::skipWhile(F f)
    {
        Linq<T> result;
        result.range = new Implemenatation::SkipWhileRange<T, F>(Implemenatation::CloneRange(range), f);
        return result;
    }

    template<typename T>
    Linq<T> Linq<T>::take(unsigned int n)
    {
        Linq<T> result;
        result.range = new Implemenatation::TakeRange<T>(Implemenatation::CloneRange(range), n);
        return result;
    }

    template<typename T>
    template<typename F>
    Linq<T> Linq<T>::takeWhile(F f)
    {
        Linq<T> result;
        result.range = new Implemenatation::TakeWhileRange<T, F>(Implemenatation::CloneRange(range), f);
        return result;
    }

    template<typename T>
    template<typename F>
    auto Linq<T>::selectMany(F f) -> Linq< decltype(f(MakeType<T>()).value()) >
    {
        typedef decltype(f(MakeType<T>()).value()) NewT;
        Linq<NewT> result;
        result.range = new Implemenatation::SelectManyRange2<NewT, T, F>(Implemenatation::CloneRange(range), f);
        return result;
    }

    template<typename T>
    template<typename F, typename G>
    auto Linq<T>::groupBy(F f, G g) -> Linq<
        std::pair<
          typename std::remove_const<decltype(f(MakeType<T>()))>::type,
          std::vector<typename std::remove_const<decltype(g(MakeType<T>()))>::type >
        >
    >
    {
        typedef typename std::remove_const<decltype(f(MakeType<T>()))>::type TKey;
        typedef typename std::remove_const<decltype(g(MakeType<T>()))>::type TValue;
        typedef	typename std::pair<TKey, std::vector<TValue> > NewT;

        Linq<NewT> result;
        result.range = new Implemenatation::GroupByRange<NewT, T, F, G, TKey, TValue>(
            Implemenatation::CloneRange(range), f, g
            );
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
    Linq<T> Linq<T>::reverse()
    {
        Linq<T> result;
        if (auto prevReverse = dynamic_cast<Implemenatation::ReverseRange<T>*>(range))
        {
            result.range = Implemenatation::CloneRange(prevReverse->srcRange());
        }
        else
        {
            result.range = new Implemenatation::ReverseRange<T>(Implemenatation::CloneRange(range));
        }
        return result;
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
        return firstOrDefault(T());
    }

    template<typename T>
    T Linq<T>::firstOrDefault(const T &defaultValue)
    {
        range->rewind();
        if (range->empty())
            return defaultValue;
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
    T Linq<T>::singleOrDefault()
    {
        return singleOrDefault(T());
    }

    template<typename T>
    T Linq<T>::singleOrDefault(const T &defaultValue)
    {
        range->rewind();
        if (range->empty())
            return defaultValue;
        auto result = range->popFront();
        if (!range->empty())
            throw new InvalidOperationException();
        return result;
    }

    template<typename T>
    T Linq<T>::last()
    {
        range->rewind();
        while(!range->empty())
        {
            T& val = range->popFront();
            if (range->empty())
                return val;
        }
        throw new InvalidOperationException();
    }

    template<typename T>
    T Linq<T>::lastOrDefault()
    {
        return lastOrDefault(T());
    }

    template<typename T>
    T Linq<T>::lastOrDefault(const T &defaultValue)
    {
        range->rewind();
        while(!range->empty())
        {
            T& val = range->popFront();
            if (range->empty())
                return val;
        }
        return defaultValue;
    }

    template<typename T>
    bool Linq<T>::empty()
    {
        range->rewind();
        return range->empty();
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
        (const T &lh, const T &rh)
        { return lh < rh; }
        );
    }

    template<typename T>
    T Linq<T>::max()
    {
        return elect(
            []
        (const T &lh, const T &rh)
        { return lh > rh; }
        );
    }

    template<typename T>
    std::pair<T, T> Linq<T>::minMax()
    {
        range->rewind();
        if (range->empty())
            throw new InvalidOperationException();
        T minVal = range->popFront();
        T maxVal = minVal;
        while(!range->empty())
        {
            T candidate = range->popFront();
            minVal = std::min(minVal, candidate);
            maxVal = std::max(maxVal, candidate);
        }
        return std::pair<T, T>(minVal, maxVal);
    }

    template<typename T>
    T Linq<T>::sum()
    {
        CleanT result = T();
        range->rewind();
        if (range->empty())
            throw new InvalidOperationException();

        while(!range->empty())
        {
            result = result + range->popFront();
        }
        return result;
    }

    template<typename T>
    template<typename F>
    auto Linq<T>::sum(F f) -> decltype(f(MakeType<T>()))
    {
        typedef typename std::remove_const<decltype(f(MakeType<T>()))>::type TResult;
        TResult result = TResult();
        if (range->empty())
            throw new InvalidOperationException();

        while(!range->empty())
        {
            result = result + f(range->popFront());
        }
        return result;
    }

    template<typename T>
    T Linq<T>::avg()
    {
        CleanT result = T();
        range->rewind();
        if (range->empty())
            throw new InvalidOperationException();

        int count = 0;
        while(!range->empty())
        {
            result = result + range->popFront();
            ++count;
        }
        if (count == 0)
            return 0;
        return result / count;
    }

    template<typename T>
    template<typename F>
    auto Linq<T>::avg(F f) -> decltype(f(MakeType<T>()))
    {
        typedef typename std::remove_const<decltype(f(MakeType<T>()))>::type TResult;
        TResult result = TResult();
        if (range->empty())
            throw new InvalidOperationException();

        int count = 0;
        while(!range->empty())
        {
            result = result + f(range->popFront());
             ++count;
        }

        if (count == 0)
            return 0;
        return result / count;
    }

    template<typename T>
    Linq<typename Linq<T>::CleanT> Linq<T>::removeConst()
    {
        Linq<CleanT> result;
        result.range = new Implemenatation::RemoveConstRange<CleanT, T>(Implemenatation::CloneRange(range));
        return result;
    }

    template<typename T>
    std::vector<T> Linq<T>::toVector()
    {
        range->rewind();
        std::vector<T> result;
        while(!range->empty())
        {
            result.push_back(range->popFront());
        }
        return result;
    }
#ifdef QVECTOR_H
    template<typename T>
    QVector<T> Linq<T>::toQVector()
    {
        range->rewind();
        QVector<T> result;
        while(!range->empty())
        {
            result.push_back(range->popFront());
        }
        return result;
    }
#endif 

    template<typename T>
    std::set<T> Linq<T>::toSet()
    {
        range->rewind();
        std::set<T> result;
        while(!range->empty())
        {
            result.insert(range->popFront());
        }
        return result;
    }

    template<typename T>
    std::list<T> Linq<T>::toList()
    {
        range->rewind();
        std::list<T> result;
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
        :Linq<T>(rh)
    {
    }

    template<typename T>
    LinqOrd<T>::LinqOrd(LinqOrd &&rh)
        :Linq<T>(rh)
    {
    }

    template<typename T>
    LinqOrd<T>& LinqOrd<T>::operator=(const LinqOrd &rh)
    {
        Linq<T> &lh = *this;
        lh = rh;
        return *this;
    }

    template<typename T>
    LinqOrd<T>& LinqOrd<T>::operator=(LinqOrd<T> && rh)
    {
        Linq<T> &lh = *this;
        lh = rh;
        return *this;
    }
    //-----------------------------------------------------------------------------
    template<typename T>
    template<typename U>
    LinqOrd<T> LinqOrd<T>::thenBy(U f)
    {
        typedef decltype(f(MakeType<T>())) TKey;
        auto comp = [](const TKey &lh, const TKey &rh) { return lh < rh ;};
        return thenBy(f, comp);
    }

    template<typename T>
    template <typename U, typename Comp>
    LinqOrd<T> LinqOrd<T>::thenBy(U f, Comp comp)
    {
        LinqOrd<T> result;
        auto orderBy = dynamic_cast<Implemenatation::BaseOrderByRange<T> *>(Implemenatation::CloneRange(this->range));
        orderBy->addComparator(f, comp, false);
        result.range = orderBy;
        return result;
    }

    template<typename T>
    template <typename U, typename Comp>
    LinqOrd<T> LinqOrd<T>::thenByDesc(U f, Comp comp)
    {
        LinqOrd<T> result;
        auto orderBy = dynamic_cast<Implemenatation::BaseOrderByRange<T> *>(Implemenatation::CloneRange(this->range));
        orderBy->addComparator(f, comp, true);
        result.range = orderBy;
        return result;
    }

    template<typename T>
    template<typename U>
    LinqOrd<T> LinqOrd<T>::thenByDesc(U f)
    {
        typedef decltype(f(MakeType<T>())) TKey;
        auto comp = [](const TKey &lh, const TKey &rh) { return lh < rh ;};
        return thenByDesc(f, comp);
    }

    //=============================================================================
    template <typename TCont>
    auto from(TCont &src) -> Linq<typename std::remove_reference<decltype(*(src.begin()))>::type>
    {
        typedef typename std::remove_reference<decltype(*(src.begin()))>::type T;
        Linq<T> result;
        result.range = new Implemenatation::ContainerRange<TCont, typename TCont::iterator, T>(src);
        return result;
    }

    template <typename TCont>
    auto from(const TCont &src) -> Linq<typename std::remove_reference<decltype(*(src.begin()))>::type>
    {
        typedef typename std::remove_reference<decltype(*(src.begin()))>::type T;
        Linq<T> result;
        result.range = new Implemenatation::ContainerRange<const TCont, typename TCont::const_iterator, T>(src);
        return result;
    }

    template <typename T, int size>
    auto from(T (&data)[size]) -> Linq<T>
    {
        Linq<T> result;
        result.range = new Implemenatation::ArrayRange<T>(data, data + size);
        return result;
    }

    template <typename T>
    auto from(T *data, size_t size) -> Linq<T>
    {
        Linq<T> result;
        result.range = new Implemenatation::ArrayRange<T>(data, data + size);
        return result;
    }

    template <typename TIter>
    auto from(TIter b, TIter e) -> Linq<typename std::iterator_traits<TIter>::value_type>
    {
        typedef typename std::iterator_traits<TIter>::value_type T;
        Linq<T> result;
        result.range = new Implemenatation::IteratorRange<TIter>(b, e);
        return result;
    }

    template <typename T>
    Linq<T> repeat(T value, unsigned int count)
    {
        Linq<T> result;
        result.range = new Implemenatation::RepeatRange<T>(value, count);
        return result;
    }

    template <typename T>
    Linq<T> empty()
    {
        Linq<T> result;
        result.range = new Implemenatation::EmptyRange<T>();
        return result;
    }
    //=============================================================================
}
