#include <cstdlib>
#include <string>
#include <sstream>
#include "semver.h"

SemVersion::SemVersion(int major, int minor, int patch)
{
    this->set(major, minor, patch);
}

SemVersion::SemVersion(int major, int minor, int patch, const char *pre_release, const char *build)
{
    this->set(major, minor, patch, pre_release, build);
}

SemVersion::SemVersion(int major, int minor, int patch, std::string &pre_release, std::string &build)
{
    this->set(major, minor, patch, pre_release, build);
}

SemVersion::SemVersion(const char *version)
{
    this->set(version);
}

SemVersion::SemVersion(std::string &version)
{
    this->set(version);
}

bool SemVersion::operator==(SemVersion &other)
{
    return (this->str() == other.str());
}

bool SemVersion::operator>(SemVersion &other)
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

void SemVersion::set(int major, int minor, int patch)
{
    this->major = major;
    this->minor = minor;
    this->patch = patch;
    this->pre_release = std::string();
    this->build = std::string();
}

void SemVersion::set(int major, int minor, int patch, const char *pre_release, const char *build)
{
    this->set(major, minor, patch);
    this->pre_release = std::string(pre_release);
    this->build = std::string(build);
}

void SemVersion::set(int major, int minor, int patch, std::string &pre_release, std::string &build)
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

void SemVersion::set(std::string &version)
{
    unsigned int firstDotPos;
    unsigned int secondDotPos;
    unsigned int dashPos;
    unsigned int plusPos;

    firstDotPos = version.find('.');
    secondDotPos = version.find('.', firstDotPos + 1);
    dashPos = version.find('-', secondDotPos + 1);
    plusPos = version.find('+', secondDotPos + 1);

    if (firstDotPos == -1 || secondDotPos == -1)
    {
        throw bad_format_exception();
    }

    this->major = std::atoi(version.substr(0, firstDotPos).c_str());
    this->minor = std::atoi(version.substr(firstDotPos + 1, secondDotPos - firstDotPos).c_str());
    if (dashPos == -1 && plusPos == -1)
    {
        this->patch = std::atoi(version.substr(secondDotPos + 1).c_str());
        this->pre_release = std::string();
        this->build = std::string();
    }
    else if (plusPos == -1)
    {
        this->patch = std::atoi(version.substr(secondDotPos + 1, dashPos - secondDotPos).c_str());
        this->pre_release = std::string(version.substr(dashPos + 1));
        this->build = std::string();
    }
    else if (dashPos == -1)
    {
        this->patch = std::atoi(version.substr(secondDotPos + 1, plusPos - secondDotPos).c_str());
        this->pre_release = std::string();
        this->build = std::string(version.substr(plusPos + 1));
    }
    else
    {
        if (plusPos < dashPos)
        {
            throw bad_format_exception();
        }

        this->patch = std::atoi(version.substr(secondDotPos + 1, dashPos - secondDotPos).c_str());
        this->pre_release = std::string(version.substr(dashPos + 1, plusPos - dashPos));
        this->build = std::string(version.substr(plusPos));
    }
}

std::string SemVersion::str()
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
