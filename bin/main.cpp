#include <string> 
#include <iostream>
#include <vector>

static int32_t counter = 0;
std::vector<bool> otv;

struct Node {
    int32_t count = -1;
    std::vector<Node*> vse;

    Node() 
        :vse(26, nullptr)
    {}

    ~Node() {
        for (auto node : vse) {
            delete node;
        }
    }
};

class Bor {
    public:
        
        Bor() {
            root = new Node();
        }

        ~Bor() {
            delete root;
        }

        void check_string(const std::string& data) {

            Node* cur = root;
            for (size_t index = 0; index < data.size(); ++index) {
                for (size_t index2 = index; index2 < data.size(); ++index2) {
                    if (cur->count != -1) {
                        otv[cur->count - 1] = true;
                    }
                    if (cur->vse[data[index2] - 'a'] == nullptr) {
                        cur = root;
                        break;
                    }
                    cur = cur->vse[data[index2] - 'a'];
                }
            }

        }

        void operator+=(const std::string& data) {
            Node* cur = root;
            ++counter;
            for (char c : data) {
                if (cur->vse[c - 'a'] == nullptr) {
                    cur->vse[c - 'a'] = new Node();
                }
                cur = cur->vse[c - 'a'];
            }
            cur->count = counter;
        }

    private:
        Node* root;
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