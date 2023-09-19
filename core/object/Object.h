//
// Created by Carter Thompson on 9/17/23.
//

#ifndef SOFTGEN_OBJECT_H
#define SOFTGEN_OBJECT_H

#ifndef NDEBUG
#include <iostream>
#endif

class Object {
public:
    std::string objectName{"Default"};
    virtual void Render() = 0;

    Object(std::string name) {
        objectName = name;
        #ifndef NDEBUG
                std::cout << "Object Created :: " << objectName << std::endl;
        #endif
    }

    virtual ~Object() {
        #ifndef NDEBUG
            std::cout << "Object Destroyed :: "  << objectName << std::endl;
        #endif
    }

};

#endif //SOFTGEN_OBJECT_H
