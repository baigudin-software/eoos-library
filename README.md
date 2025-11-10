# EOOS Library
---

**EOOS copyrights reserved in [Rospatent Federal Service for Intellectual Property](https://www1.fips.ru/registers-doc-view/fips_servlet?DB=EVM&DocNumber=2017664105&TypeFile=html), Russian Federation**

This EOOS Library is an elaborated EOOS software component which is a **C++ template library** of
safety-critical software. The library is developed in **C++98** programming language and supports
features of **C++11** programming language depending on compiler options that
**allows to consider wide criteria when selecting a suitable programming language** on initiation step
of product development.

EOOS Library is developed within **ISO C++ standards**, complied with **MISRA C++:2008** and
**AUTOSAR C++14 Coding Guidelines**, and relies on **ISO 26262** that means applications based on EOOS
**can be used in critical and safety-related systems**.

---

## 1. About The Project

This repository is a software module of EOOS which contains C++ template classes of EOOS Library
to access to EOOS System and to simplify and speedup development of EOOS applications.
The component is included to all the EOOS Projects.

#### 1.1. Dependencies on other packages

To build the package, there are dependencies on other EOOS packages
that must be found by the build system:

- eoos::api
- eoos::sys
