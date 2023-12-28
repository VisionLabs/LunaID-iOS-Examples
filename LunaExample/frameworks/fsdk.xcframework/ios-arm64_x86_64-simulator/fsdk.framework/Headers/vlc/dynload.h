#pragma once

namespace vlc
{
    struct VLC_LIBRARY_API Library
    {
		using LibraryHandle = void*;

    	LibraryHandle _handle = nullptr;

    	Library() = default;
    	~Library();

    	Library(const Library&) = delete;
    	Library& operator = (const Library&) = delete;

    	Library(Library&& src)
    	{
    		std::swap(_handle, src._handle);
    	}

    	Library& operator = (Library&& src)
    	{
    		std::swap(_handle, src._handle);
    		return *this;
    	}

    	explicit operator bool() const
    	{
    		return _handle != nullptr;
    	}

    	bool load(const char* name, bool auto_ext = true);

    	bool load(const char* name, bool auto_ext, std::string& err_msg);

    	bool load(const char* name, std::string& err_msg)
		{
			return load(name, true, err_msg);
		}

    	void* symbol(const char* name);

    	template<typename F> F get(const char* name)
    	{
            return reinterpret_cast<F>(symbol(name));
    	}
    };
}
