#ifndef __LowPassFilter_Standalone__GeneralExceptions__
#define __LowPassFilter_Standalone__GeneralExceptions__

//#include <iostream>
#include <exception>

class NotFoundFile_Exception : public std::exception {
    const char* what() const noexcept {return "NotFoundFile_Exception: File could not be opened!\n";}
};

class WrongFunArgVal_Exception : public std::exception {
    const char* what() const noexcept {return "WrongFunArgVal_Exception: Wrong function arguments value exception!\n";}
};

class IndexExceedMaxValue_Exception : public std::exception {
    const char* what() const noexcept {return "IndexExceedMaxValue_Exception: .. !\n";}
};

class NotExpectedProcessingResult_Exception : public std::exception {
    const char* what() const noexcept {return "NotExpectedProcessingResult_Exception: output differ than expected !\n";}
};

class UnkownInternalProcessingError_Exception : public std::exception {
    const char* what() const noexcept {return "UnkownInternalProcessingError_Exception: .. !\n";}
};

class TemplateDoesNotSupportThatType_Exception: public std::exception {
    const char* what() const noexcept {return "Class template does not support type for which was created. Read which types supports tempalte\n";}
};

#endif /* defined(__LowPassFilter_Standalone__GeneralExceptions__) */