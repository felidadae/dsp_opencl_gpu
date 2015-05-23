//
//  IParameterizable.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 30.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef FilterModule_Standalone_IParameterizable_h
#define FilterModule_Standalone_IParameterizable_h

#include <string>
#include <map>
#include <boost/any.hpp>



template<class KeyClass>
class Parameters {
public:
    
    template<class ValueClass>
    ValueClass operator [] (const KeyClass& paramName) {
        ValueClass v;
        getValue(paramName, v);
        return v;
    }
    
    template<class ValueClass>
    bool/*return true if there was such a parameter*/ getValue(const KeyClass& paramName, ValueClass& /*out*/ value) const {
        if(params_.find(paramName) == params_.end())
            return false;
        value =  boost::any_cast<ValueClass> (params_.find(paramName)->second);
        return true;
    }
    
    template<class ValueClass>
    void add(const KeyClass paramName, ValueClass value) {
        params_[paramName] = boost::any(value);
    }
    
private:
    std::map <KeyClass, boost::any> params_;
};



template<class ParametersClass>
class IParameterizable {
public:
    ~IParameterizable() {}

    virtual void setParameters(const ParametersClass& params) = 0;
};


#endif
