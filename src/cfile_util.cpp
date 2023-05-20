// Copyright (c) 2021-2023 The MVC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cfile_util.h>

#ifdef WIN32
#include <io.h>
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif // _MSC_VER
#else
#include <unistd.h>
#endif // WIN32

UniqueFileDescriptor& UniqueFileDescriptor::operator=(UniqueFileDescriptor&& that) noexcept
{
    if(this != &that)
    {   
        // Close our current fd
        Reset();
        // Take over ownership of the other fd
        mFd = that.Release();
    }

    return *this;
}

UniqueFileDescriptor::~UniqueFileDescriptor() noexcept
{
    // Close our fd (if we have one)
    Reset();
}

// Release ownership of the managed file-descriptor
int UniqueFileDescriptor::Release() noexcept
{
    int fd { mFd };
    mFd = -1;
    return fd;
}

// Free and close our file-descriptor
void UniqueFileDescriptor::Reset() noexcept
{
    if(mFd >= 0)
    {   
        close(mFd);
        mFd = -1;
    }
}

