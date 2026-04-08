
#ifndef INFOH502_CPP_UTILS_H
#define INFOH502_CPP_UTILS_H

#define ERROR(msg) \
    std::cerr << "[ERROR] " << __FILE__ << ":" << __LINE__ \
    << " (" << __func__ << ") - " << msg << std::endl;

#define FATAL(msg) \
    do { \
    std::cerr << "[FATAL] " << __FILE__ << ":" << __LINE__ \
    << " (" << __func__ << ") - " << msg << std::endl; \
    std::exit(EXIT_FAILURE); \
    } while (0)

#endif //INFOH502_CPP_UTILS_H
