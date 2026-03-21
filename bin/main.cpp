#include <string> 
#include <iostream>
#include <vector>

template<typename key, typename value>
requires std::totally_ordered<key>
class AbstractMap {
    public:
        AbstractMap(key key_, value value_, auto compare = [](key key1, key key2) {
            return key1 < key2;
        }) 
        {}


        bool operator==(const AbstractMap& other) const 
        requires std::equality_comparable<key> && std::equality_comparable<value>
        {
            
        }

    private:
        Node* root_; // корень дерева
        auto compare_; // функция сравнения
};

int main(int argc, char* argv[]) {
    std::string data;
    size_t coll;
    Bor bor;
    std::cin >> data >> coll;
    for (size_t index = 0; index < coll; ++index) {
        std::cin >> data;
        otv.push_back(false);
        bor += data;
    }
    bor.check_string(data);
    for (size_t index = 0; index < otv.size(); ++index) {
        if (otv[index]) {
            std::cout << "Yes" << std::endl;
        } else {
            std::cout << "No" << std::endl;
        }
    }
}