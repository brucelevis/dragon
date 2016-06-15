
#ifndef _Z_MACRO_H_
#define _Z_MACRO_H_

#define Z_SYNTHESIZE_READONLY(varType, varName) \
protected: varType varName##_; \
public: varType get_##varName(void) const { return varName##_; }

#define Z_SYNTHESIZE(varType, varName) \
protected: varType varName##_; \
public: varType get_##varName(void) const { return varName##_; } \
public: void set_##varName(varType varName) { varName##_ = varName; } 

#define Z_SYNTHESIZE_BY_REF(varType, varName) \
protected: varType varName##_; \
public: const varType& get_##varName(void) const { return varName##_; } \
public: void set_##varName(varType varName) { varName##_ = varName; } 

#define Z_PROPERTY_READONLY(varType, varName) \
protected: varType varName##_; \
public: varType get_##varName(void);

#define Z_PROPERTY(varType, varName) \
protected: varType varName##_; \
public: varType get_##varName(void); \
public: void set_##varName(varType varName);

#define Z_PROPERTY_BY_REF(varType, varName) \
protected: varType varName##_; \
public: const varType& get_##varName(void); \
public: void set_##varName(varType varName); 

#endif

