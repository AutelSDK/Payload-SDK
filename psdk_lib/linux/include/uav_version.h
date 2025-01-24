#ifndef __UAV_VERSION_HPP__
#define __UAV_VERSION_HPP__

#ifdef __cplusplus
extern "C" {
#endif

extern const char *UAV_Version(void);
extern const int UAV_Version_Major(void);
extern const int UAV_Version_Minor(void);
extern const int UAV_Version_Patch(void);
extern const int UAV_Version_Build(void);

#ifdef __cplusplus
}
#endif

#endif
