#include <cstdlib>
#include <string>
#include <sstream>
#include "semver.h"

SemVersion::SemVersion(uint major, uint minor, uint patch)
{
    this->set(major, minor, patch);
}

SemVersion::SemVersion(uint major, uint minor, uint patch, const char *pre_release, const char *build)
{
    this->set(major, minor, patch, pre_release, build);
}

SemVersion::SemVersion(uint major, uint minor, uint patch, const std::string &pre_release, const std::string &build)
{
    this->set(major, minor, patch, pre_release, build);
}

SemVersion::SemVersion(const char *version)
{
    this->set(version);
}

SemVersion::SemVersion(const std::string &version)
{
    this->set(version);
}

bool SemVersion::operator==(const SemVersion &other) const
{
    return (this->str() == other.str());
}

bool SemVersion::operator>(const SemVersion &other) const
{
    if (this->major > other.major)
    {
        return true;
    }
    else if (this->major < other.major)
    {
        return false;
    }

    if (this->minor > other.minor)
    {
        return true;
    }
    else if (this->minor < other.minor)
    {
        return false;
    }

    if (this->patch > other.patch)
    {
        return true;
    }
    else if (this->patch < other.patch)
    {
        return false;
    }

    return false;
}

void SemVersion::set(uint major, uint minor, uint patch)
{
    this->major = major;
    this->minor = minor;
    this->patch = patch;
    this->pre_release = std::string();
    this->build = std::string();
}

void SemVersion::set(uint major, uint minor, uint patch, const char *pre_release, const char *build)
{
    this->set(major, minor, patch);
    this->pre_release = std::string(pre_release);
    this->build = std::string(build);
}

void SemVersion::set(uint major, uint minor, uint patch, const std::string &pre_release, const std::string &build)
{
    this->set(major, minor, patch);
    this->pre_release = std::string(pre_release);
    this->build = std::string(build);
}

void SemVersion::set(const char *version)
{
    std::string strVersion(version);

    this->set(strVersion);
}

void SemVersion::set(const std::string &version)
{
    // regex to capture semantic version
    // the regex matches case insensitive
    // (1) major version 0 or unlimited number
    // (2) . minor version (0 or unlimited number)
    // (3) . patch version (0 or unlimited number)
    // (4) optional pre-release following a dash consisting of a alphanumeric letters
    //     and hyphens using a non-capture subclause to exclude the dash from the 
    //     pre-release string
    // (5) optional build following a plus consisting of alphanumeric letters and
    //     hyphens using a non-capture subclause to exclude the plus from the build string
    auto semver_regex = std::regex("^(0|[1-9][0-9]*)"   // (1)
                                   "\\.(0|[1-9][0-9]*)" // (2)
                                   "\\.(0|[1-9][0-9]*)" // (3)
                                   "(?:\\-([0-9a-z-]+[\\.0-9a-z-]*))?" // (4)
                                   "(?:\\+([0-9a-z-]+[\\.0-9a-z-]*))?" // (5)
                                   ,
                                   std::regex_constants::ECMAScript |
                                   std::regex_constants::icase);

    auto pieces_match = std::smatch();
    if(std::regex_match(version, pieces_match, semver_regex))
    {
      set(std::atoi(pieces_match[1].str().c_str()),
          std::atoi(pieces_match[2].str().c_str()),
          std::atoi(pieces_match[3].str().c_str()),
          pieces_match[4],
          pieces_match[5]);
    }
    else
    {
      throw bad_format_exception();
    }
}

std::string SemVersion::str() const
{
    std::ostringstream stream;

    stream << this->major << '.' << this->minor << '.' << this->patch;

    if (this->pre_release.length() > 0)
    {
        stream << '-' << this->pre_release;
    }
    if (this->build.length() > 0)
    {
        stream << '+' << this->build;
    }

    return stream.str();
}
