# Copilot Instructions

## 项目指南
- 用户要求：对于 `#import` 已提供的 COM 智能指针（如 `mscorlib::_AppDomainPtr`、`mscorlib::_ObjectHandlePtr`），不要替换为 WRL `ComPtr`；WRL `ComPtr` 仅用于原生 COM 接口的 RAII。
