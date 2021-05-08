//
// Created by Centauria V. CHEN on 2021/5/7.
//

#ifndef PHOENIXKILLER_PLATFORM_H
#define PHOENIXKILLER_PLATFORM_H

#ifdef WIN32
#define LINESEP "\r\n"
#elif defined(MACOSX)
#define LINESEP "\r"
#else
#define LINESEP "\n"
#endif

#endif //PHOENIXKILLER_PLATFORM_H
