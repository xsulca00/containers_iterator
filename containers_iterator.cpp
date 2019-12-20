#include <string>
#include <tuple>
#include <iostream>
#include <map>
#include <vector>
#include <deque>
#include <list>

using namespace std;

template<int ...> struct seq {};
template<int N, int... S> struct gens : gens<N - 1, N - 1, S...> {};
template<int ...S> struct gens<0, S...>{ using type = seq<S...> ; };

template<typename T>
using ItType = typename T::iterator;

template<typename T>
using ValueType = typename T::value_type;

template<typename... T>
class Iterator {
public:    
    static constexpr auto Size = sizeof...(T);
    static constexpr seq Seq = typename gens<Size>::type{};

    Iterator(T&... t) : _tuple{t...} {
        if (!CheckSizes(Seq)) throw runtime_error{"Containers must have same size"};
    }

    class Iterators {
    public:
        Iterators(ItType<T>... tt) : t{tt...} {}

        Iterators& operator++() { Increment(Seq); return *this; }

        template<int ...S>
        tuple<ValueType<T>...> MakeTuple(seq<S...>) { return {*get<S>(t)...}; }

        template<int ...S>
        tuple<ValueType<T>...> MakeTuple(seq<S...>) const { return {*get<S>(t)...}; }
        
        tuple<ValueType<T>...> operator*() { return MakeTuple(Seq); }
        tuple<ValueType<T>...> operator*() const { return MakeTuple(Seq); }
    private:
        tuple<ItType<T>...> t;

        template<int ...S>
        void Increment(seq<S...>) { (++get<S>(t), ...); }
    };

    friend bool operator!=(const Iterators& l, const Iterators& r) { return *l != *r; }
   
    Iterators begin() { return MakeBegins(Seq); }
    Iterators end() { return MakeEnds(Seq); }
private:
    tuple<T...> _tuple;

    template<int ...S>
    Iterators MakeBegins(seq<S...>) { return {std::begin(get<S>(_tuple))...}; }
    template<int ...S>
    Iterators MakeEnds(seq<S...>) { return {std::end(get<S>(_tuple))...}; }
    template<int ...S>
    bool CheckSizes(seq<S...>) { auto size {std::size(get<0>(_tuple))}; return (... && (size == get<S>(_tuple).size())); }
};

int main() {
    vector v1 {1,2,3};
    vector v2 {1,2,3};
    vector v3 {1,2,3};
    deque d1 {'a', 'b', 'c'};
    list l1 {4.5, 8.7, 3.4};
    list l2 {"adam", "dominiku", "alex"};
	array a {1,2,3};

    for (auto [a,b,c,d,e,f,g] : Iterator{v1,v2,v3,l1,l2,d1,a} ) {
        cout << a << ' ' << b << ' ' << c << ' ' << d << ' ' << e << ' ' << f << ' ' << g << '\n';
    }
}
