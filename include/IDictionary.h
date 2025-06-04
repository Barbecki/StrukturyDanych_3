#pragma once
// plik bazowy dla interfejsu słownika
class IDictionary {
public:
    virtual void insert(int key, int value) = 0;
    virtual void remove(int key) = 0;
    virtual bool find(int key, int& value) const = 0;
    virtual ~IDictionary() {}
};
