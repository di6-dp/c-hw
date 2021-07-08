//1. удалена директива pragma once
//1.1 удалена pragma once/ifndef

#include <vector>
#include <random>
#include <cassert>
#include <algorithm>

template<class T>
class randomized_queue;

template<class T, bool is_const>
class randomized_queue_iterator {
public:
    template<class P>
    using add_constness = std::conditional_t<is_const, std::add_const_t<P>, P>;

    using difference_type = std::ptrdiff_t;
    using value_type = add_constness<T>;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = std::bidirectional_iterator_tag;



    randomized_queue_iterator(add_constness<randomized_queue<T>> *queue, const std::size_t pos = 0)
            : randomized_queue_container(queue), index(pos) {
        generate_transposition(queue->size());
    }

    randomized_queue_iterator &operator++() {
        assert (index < randomized_queue_container->size() && "Out of range, please review your code. Maybe it is end iterator.");//4. if убран, добавлен assert
        index++;
        return *this;
    }

    randomized_queue_iterator operator++(int) {
        auto tmp = *this;
        operator++();
        return tmp;
    }

    randomized_queue_iterator &operator--() {
        assert (index > 0 && "Out of range, please review your code. Maybe it is begin iterator.");//4. if убран, добавлен assert
        index--;
        return *this;
    }

    randomized_queue_iterator operator--(int) {
        auto tmp = *this;
        operator--();
        return tmp;
    }

     reference operator*() const {
        assert(index < transposition.size() && index >= 0 && "Out of range, please review your code. Maybe it is end or begin iterator."); //5. добавлены комментарии к assert
        return (randomized_queue_container)->elements[transposition[index]];
    }

    pointer operator->() const {
        assert(index < transposition.size() &&  index >= 0 && "Out of range, please review your code. Maybe it is end or begin iterator.");//5. добавлен assert
        return &*(randomized_queue_container)->elements[transposition[index]];
    }

    friend bool operator==(const randomized_queue_iterator &lit, const randomized_queue_iterator &rit) {
        return (lit.index == rit.index) && (lit.randomized_queue_container == rit.randomized_queue_container);
    }

    friend bool operator!=(const randomized_queue_iterator &lit, const randomized_queue_iterator &rit) {
        return !(lit == rit);
    }

private:
    void generate_transposition(size_t size) {//3. метод сделан private
        transposition.reserve(size + 1); //2. резервируется вектор размера size + 1
        for (size_t i = 0; i < size; i++) {
            transposition.push_back(i);
        }
        std::shuffle(transposition.begin(), transposition.end(), randomized_queue_container->mt);
        transposition.push_back(size);
    }

    std::vector<size_t> transposition;
    add_constness<randomized_queue<T>> *randomized_queue_container = nullptr;
    size_t index = 0;
};

template<class T>
class randomized_queue {

public:
    using iterator  = randomized_queue_iterator<T, false>;
    using const_iterator = randomized_queue_iterator<T, true>;

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, size());
    }

    const_iterator cbegin() const {
        return const_iterator(this, 0);
    }

    const_iterator cend() const {
        return const_iterator(this, size());
    }

    const_iterator begin() const {
        return const_iterator(this, 0);
    }

    const_iterator end() const {
        return const_iterator(this, size());
    }

    randomized_queue() : mt(rd()) {}

    bool empty() const {
        return size() == 0;
    }

    std::size_t size() const {
        return elements.size();
    }

    void enqueue(const T &x) {
        elements.push_back(x);
    }

    void enqueue(T &&x) {
        elements.push_back(std::move(x));//6. std::forward заменено на std::move 6.1 emplace_back заменен на push_back
    }

    std::size_t random_number() const {
        assert(size() > 0);
        std::uniform_int_distribution<int> uid(0, size() - 1);
        return uid(mt) ;
        //7. Ограничение на uid оставлено, но убрано лишнее деление по модулю. В комментариях снизу иное решение, с делением по модулю, но без верхней границы для uid.
        //std::uniform_int_distribution<int> uid(0);
        // return uid(mt) % size();;

    }

    const T &sample() const {
        return elements[random_number()];
    }

    T &sample() {
        return elements[random_number()];
    }

    T dequeue() {//8. swap убран, убран лишний std::move для последнего элемента
        size_t ind = random_number();
        T result = std::move(elements[ind]);
        elements[ind] = std::move(elements.back());
        elements.pop_back();
        return result;
    }



private://9. поля queue сделаны private, для этого каждый из возможных итераторов объявлен как friend class
    friend class randomized_queue_iterator<T, true>;
    friend class randomized_queue_iterator<T, false>;


    std::vector<T> elements;
    mutable std::random_device rd;
    mutable std::mt19937 mt;



};