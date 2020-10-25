#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include <vector>
#include <utility>

template <typename T>
void sortFull(std::vector<T> &vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        for (int j = 0; j < vec.size() - i - 1; j++)
        {
            if (vec.at(j) > vec.at(j + 1))
                std::swap(vec.at(j), vec.at(j + 1));
        }
    }
}

template <typename T>
std::pair<int, int> sortStep(std::vector<T> &vec, std::pair<int, int> &current)
{
    std::pair<int, int> ret = std::make_pair(-1, -1);
    if (current.first >= vec.size()) return ret;
    if(current.second < vec.size() - current.first - 1)
    {
        if (vec.at(current.second) > vec.at(current.second + 1))
        {
            std::swap(vec.at(current.second), vec.at(current.second + 1));
            ret.first = current.second;
            ret.second = current.second + 1;
        }
        current.second++;
    }
    else
    {
        current.first++;
        current.second = 0;
    }
    return ret;
}

template <typename T>
bool isSorted(std::vector<T> &vec)
{
    T last = vec.at(0);
    for (int i = 0; i < vec.size() - 1; i++)
    {
        if (vec.at(i) > vec.at(i + 1))
            return false;
    }
    return true;
}

#endif