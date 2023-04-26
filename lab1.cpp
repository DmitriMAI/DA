#include <iostream>
#include <memory>
#include<sstream>
#include <cstring>

#include<vector>
#include <algorithm>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";


const long long SOME_BIG_TEN_IN_TEN = 10000000000;
const long long SOME_BIG_TEN_IN_SEVEN = 10000000;
const long long SOME_BIG_TEN_IN_SIX = 1000000;
const int SOME_BIG_TEN_IN_FOUR = 10000;
const int SOME_USEFUL_SEVEN = 7;
const int SOME_USEFUL_FOUR = 4;
const int SOME_USEFUL_THREE = 3;
const int SOME_USEFUL_SIXTEEN_ONES = 65535;
const int SOME_USEFUL_SIXTEEN_ONES_PLUS_ONE = 65536;
const int SOME_USEFUL_SIXTEEN_ONES_PLUS_TWO = 65537;
const int SOME_USEFUL_SIXTEEN = 16;
const int SOME_USEFUL_TEN = 10;
const int SOME_USEFUL_ELEVEN = 11;
struct MyPair{
    long long number;
    char c;
    std::string val;
    MyPair(){}
    MyPair(long long number, int c, std::string val): number(number), c(c), val(val) {}

};

template <typename T>
struct TMyallocator{
    T* Allocate(size_t n) {
        return reinterpret_cast<T*>(new char[n * sizeof(T)]);
    }
    void Deallocate(T* ptr, size_t){
        delete[] reinterpret_cast<char*>(ptr);
    }
    
    template<typename... Args>
    void Construct(T* ptr, const  Args&... args){
        new(ptr) T(args...);
    }

    void Destroy(T* ptr){
        ptr->~T();
    }
};

template<typename T, typename Alloc = TMyallocator<T>>
class TVector{
private:
    T* arr;
    size_t sz;
    size_t cap;
    Alloc alloc;
public:
    TVector(): sz(0), cap(SOME_BIG_TEN_IN_SIX){
        arr = new T[SOME_BIG_TEN_IN_SIX];
    }
    TVector(int n): sz(n), cap(n){
        arr = new T[n];
        for (int i = 0; i < n ; ++i){
            arr[i] = MyPair(0,0,"");
        }
    }
    void Reserve (size_t n){
        T* newarr = alloc.Allocate(n);
        for (size_t i = 0; i < cap; ++i) {
            newarr[i] = arr[i];
        }
        delete[] arr;
        arr = newarr;
        cap = n;
    }
    void Push_back(const T& value){
        if (sz >= cap) {
            Reserve(sz + SOME_BIG_TEN_IN_FOUR);
        }
        arr[sz] = value;
        ++sz;
    }
    void Pop_back(){
        --sz;
    }
    T& operator [](size_t i){
        return arr[i];
    }
    const T& operator [](size_t i)const {
        return arr[i];
    }
    T& At (size_t i){
        if (i >= sz){
            throw std::out_of_range("...");
        }
        return arr[i];
    }
    int Size(){
        return sz;
    }
    void Print(){
        for (int i = 0; i < sz; ++i){
            std::cout << arr[i].number << '\t' << arr[i].val << '\n';
        }
    }
};

int Get_by_mask_long_long(long long number, int n){
    int i = SOME_USEFUL_SIXTEEN_ONES;
    for (int i = 0; i < n; ++i){
        number = number >> SOME_USEFUL_SIXTEEN;
    }
    return i & number;
}


void Counting_sort_int_numbers(TVector<MyPair>& v,TVector<MyPair>& second,  int number){
    int* b = new int [SOME_USEFUL_SIXTEEN_ONES_PLUS_TWO];
    for(int i = 0; i <= SOME_USEFUL_SIXTEEN_ONES_PLUS_ONE; ++i){
        b[i] = 0;
    }
    for(int j = 0; j < v.Size(); j++){
        b[Get_by_mask_long_long(v[j].number, number)]++;
    }
    int sum = 0;
    for(int k = 0; k < SOME_USEFUL_SIXTEEN_ONES_PLUS_ONE; ++k){
        sum += b[k];
        b[k] = sum;
    }
    for (int l = v.Size()-1; l >= 0; --l){
        int q = 0;
        b[Get_by_mask_long_long(v[l].number, number)]--;
        q = b[Get_by_mask_long_long(v[l].number, number)];
        std::swap(v[l].c, second[q].c);
        std::swap(v[l].number, second[q].number);
        std::swap(second[q].val, v[l].val);
    }
    delete[] b;
}

void LSDSort(TVector<MyPair>& v){
    TVector<MyPair> c(v.Size());
    for (int j = 0; j < SOME_USEFUL_FOUR ; ++j){
        if ((j & 1) == 0){
            Counting_sort_int_numbers(v, c, j);
        }
        else{
            Counting_sort_int_numbers(c, v, j);
        }
    }
}

int Len_of_int(long long number){
    int temp = 0;
    while(number != 0){
        temp++;
        number /= SOME_USEFUL_TEN;
    }
    return temp;
}

bool cmp(MyPair lhs, MyPair rhs) {
    return lhs.number < rhs.number;
}

int main () {
    std::vector<MyPair> stl_v;
	std::ios_base::sync_with_stdio(false);
 	std::cin.tie(NULL);
    TVector<MyPair> v;
    std::string line;
    std::string key;
    std::string val;
    long long last_numbers = 0;
    while (std::getline(std::cin, line)){
        std::stringstream ss;
        ss << line;
        if (line.empty()) {
            continue;
        }
        ss >> key;
        if (key[0] != '+'){
            throw ;
        }
        key.erase(key.begin());
        last_numbers = 0;
        char first_three_len = 0;
        for (int i = 0; i < SOME_USEFUL_FOUR; ++i){
            if (key[i] == '-'){
                break;
            }
            last_numbers *= SOME_USEFUL_TEN;
            last_numbers += key[i] - '0';
            first_three_len++;
        }
        for (size_t i = key.size() - SOME_USEFUL_ELEVEN; i < key.size(); ++i){
            if (key[i] == '-'){
                continue;
            }
            last_numbers *= SOME_USEFUL_TEN;
            last_numbers += key[i] - '0';
        }
        ss >> val;
        MyPair temppair (last_numbers, first_three_len, val);
        v.Push_back(temppair);
        stl_v.push_back(temppair);
    }
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    LSDSort(v);
    auto end_ts = std::chrono::system_clock::now();
    uint64_t counting_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    start_ts = std::chrono::system_clock::now();
    std::stable_sort(std::begin(stl_v), std::end(stl_v), cmp);
    end_ts = std::chrono::system_clock::now();

    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "LSD sort time: " << counting_sort_ts << DURATION_PREFIX << std::endl;
    std::cout << "STL stable sort time: " << stl_sort_ts << DURATION_PREFIX << std::endl;
    return 0;
    int len_of_number;
    for (int i = 0; i < v.Size(); ++i){
        std::cout << "+";
        int j = v[i].number/SOME_BIG_TEN_IN_TEN;
        if (Len_of_int(j) != v[i].c){
            if (j == 0){
                v[i].c--;
            }
            for (int k = 0; k < std::abs(Len_of_int(j)-v[i].c); ++k){
                std::cout << '0';
            }
        }
        std::cout << v[i].number/SOME_BIG_TEN_IN_TEN;
        std::cout << "-";
        len_of_number = Len_of_int(v[i].number%SOME_BIG_TEN_IN_TEN/SOME_BIG_TEN_IN_SEVEN);
        if (len_of_number == 0){
            len_of_number++;
        }
        for (int i = len_of_number; i < SOME_USEFUL_THREE; ++i){
            std::cout << '0';
        }
        std::cout << v[i].number %SOME_BIG_TEN_IN_TEN/SOME_BIG_TEN_IN_SEVEN << '-';
        len_of_number = Len_of_int(v[i].number%SOME_BIG_TEN_IN_SEVEN);
        if (len_of_number == 0){
            len_of_number++;
        }
        for (int i = len_of_number; i < SOME_USEFUL_SEVEN; ++i){
            std::cout << '0';
        }
        std::cout <<  v[i].number %SOME_BIG_TEN_IN_SEVEN;
        std::cout << '\t' << v[i].val << '\n';
    }
    return 0;
}
