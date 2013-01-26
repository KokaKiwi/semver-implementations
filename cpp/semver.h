#ifndef SEMVER_H_
#define SEMVER_H_

#include <stdexcept>
#include <string>

class SemVersion
{
public:
    SemVersion(int, int, int);
    SemVersion(int, int, int, const char *, const char *);
    SemVersion(int, int, int, const std::string &, const std::string &);
    SemVersion(const char *);
    SemVersion(const std::string &);

    bool operator==(const SemVersion &) const;
    bool operator>(const SemVersion &) const;

    void set(int, int, int);
    void set(int, int, int, const char *, const char *);
    void set(int, int, int, const std::string &, const std::string &);
    void set(const char *);
    void set(const std::string &);

    std::string str(void) const;

    int major;
    int minor;
    int patch;
    std::string pre_release;
    std::string build;
};

class bad_format_exception: public std::runtime_error
{
public:
    bad_format_exception(void): std::runtime_error("Bad version format.") {};
};

#endif /* SEMVER_H_ */
