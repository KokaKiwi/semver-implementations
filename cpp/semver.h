#ifndef SEMVER_H_
#define SEMVER_H_

#include <exception>
#include <string>

class SemVersion
{
public:
    SemVersion(int, int, int);
    SemVersion(int, int, int, const char *, const char *);
    SemVersion(int, int, int, std::string &, std::string &);
    SemVersion(const char *);
    SemVersion(std::string &);

    bool operator==(SemVersion &);
    bool operator>(SemVersion &);

    void set(int, int, int);
    void set(int, int, int, const char *, const char *);
    void set(int, int, int, std::string &, std::string &);
    void set(const char *);
    void set(std::string &);

    std::string str(void);

    int major;
    int minor;
    int patch;
    std::string pre_release;
    std::string build;
};

class bad_format_exception: public std::exception
{
    virtual const char *what() const throw()
    {
        return "Bad version format.";
    }
};

#endif /* SEMVER_H_ */
