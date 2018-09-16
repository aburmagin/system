// Windows implementation of system_error_category
//
// Copyright Beman Dawes 2002, 2006
// Copyright (c) Microsoft Corporation 2014
// Copyright 2018 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See library home page at http://www.boost.org/libs/system

#include <boost/winapi/error_codes.hpp>
#include <boost/winapi/error_handling.hpp>
#include <boost/winapi/character_code_conversion.hpp>

//

namespace boost
{

namespace system
{

namespace detail
{

inline std::string system_category_message_win32( int ev )
{
    using namespace boost::winapi;

    std::wstring buf( 128, wchar_t() );

    for( ;; )
    {
        DWORD_ retval = boost::winapi::FormatMessageW(
            FORMAT_MESSAGE_FROM_SYSTEM_ | FORMAT_MESSAGE_IGNORE_INSERTS_,
            NULL,
            ev,
            MAKELANGID_( LANG_NEUTRAL_, SUBLANG_DEFAULT_ ), // Default language
            &buf[0],
            static_cast<DWORD_>( buf.size() ),
            NULL
        );

        if( retval > 0 )
        {
            buf.resize(retval);
            break;
        }
        else if( boost::winapi::GetLastError() != ERROR_INSUFFICIENT_BUFFER_ )
        {
            return "Unknown error";
        }
        else
        {
            buf.resize( buf.size() + buf.size() / 2 );
        }
    }

    int num_chars = static_cast<int>( buf.size() + 1 ) * 2;

    boost::winapi::LPSTR_ narrow_buffer =
#if defined(__GNUC__)
        (boost::winapi::LPSTR_)__builtin_alloca( num_chars );
#else
        (boost::winapi::LPSTR_)_alloca( num_chars );
#endif

    if( boost::winapi::WideCharToMultiByte( CP_ACP_, 0, buf.c_str(), -1, narrow_buffer, num_chars, NULL, NULL ) == 0 )
    {
        return "Unknown error";
    }

    std::string str( narrow_buffer );

    while( !str.empty() && ( str[ str.size() - 1 ] == '\n' || str[ str.size() - 1 ] == '\r' ) )
    {
        str.erase( str.size() - 1 );
    }

    if( str.size() && str[ str.size() - 1 ] == '.' )
    {
        str.erase( str.size() - 1 );
    }

    return str;
}

inline error_condition system_category_default_error_condition_win32( int ev ) BOOST_NOEXCEPT
{
    // When using the Windows Runtime, most system errors are reported as HRESULTs.
    // We want to map the common Win32 errors to their equivalent error condition,
    // whether or not they are reported via an HRESULT.

#define BOOST_SYSTEM_FAILED(hr)           ((hr) < 0)
#define BOOST_SYSTEM_HRESULT_FACILITY(hr) (((hr) >> 16) & 0x1fff)
#define BOOST_SYSTEM_HRESULT_CODE(hr)     ((hr) & 0xFFFF)
#define BOOST_SYSTEM_FACILITY_WIN32       7

    if( BOOST_SYSTEM_FAILED( ev ) && BOOST_SYSTEM_HRESULT_FACILITY( ev ) == BOOST_SYSTEM_FACILITY_WIN32 )
    {
        ev = BOOST_SYSTEM_HRESULT_CODE( ev );
    }

#undef BOOST_SYSTEM_FAILED
#undef BOOST_SYSTEM_HRESULT_FACILITY
#undef BOOST_SYSTEM_HRESULT_CODE
#undef BOOST_SYSTEM_FACILITY_WIN32

    using namespace boost::winapi;
    using namespace errc;

    // Windows system -> posix_errno decode table
    // see WinError.h comments for descriptions of errors

    switch ( ev )
    {
    case 0: return make_error_condition( success );

    case ERROR_ACCESS_DENIED_: return make_error_condition( permission_denied );
    case ERROR_ALREADY_EXISTS_: return make_error_condition( file_exists );
    case ERROR_BAD_UNIT_: return make_error_condition( no_such_device );
    case ERROR_BUFFER_OVERFLOW_: return make_error_condition( filename_too_long );
    case ERROR_BUSY_: return make_error_condition( device_or_resource_busy );
    case ERROR_BUSY_DRIVE_: return make_error_condition( device_or_resource_busy );
    case ERROR_CANNOT_MAKE_: return make_error_condition( permission_denied );
    case ERROR_CANTOPEN_: return make_error_condition( io_error );
    case ERROR_CANTREAD_: return make_error_condition( io_error );
    case ERROR_CANTWRITE_: return make_error_condition( io_error );
    case ERROR_CURRENT_DIRECTORY_: return make_error_condition( permission_denied );
    case ERROR_DEV_NOT_EXIST_: return make_error_condition( no_such_device );
    case ERROR_DEVICE_IN_USE_: return make_error_condition( device_or_resource_busy );
    case ERROR_DIR_NOT_EMPTY_: return make_error_condition( directory_not_empty );
    case ERROR_DIRECTORY_: return make_error_condition( invalid_argument ); // WinError.h: "The directory name is invalid"
    case ERROR_DISK_FULL_: return make_error_condition( no_space_on_device );
    case ERROR_FILE_EXISTS_: return make_error_condition( file_exists );
    case ERROR_FILE_NOT_FOUND_: return make_error_condition( no_such_file_or_directory );
    case ERROR_HANDLE_DISK_FULL_: return make_error_condition( no_space_on_device );
    case ERROR_INVALID_ACCESS_: return make_error_condition( permission_denied );
    case ERROR_INVALID_DRIVE_: return make_error_condition( no_such_device );
    case ERROR_INVALID_FUNCTION_: return make_error_condition( function_not_supported );
    case ERROR_INVALID_HANDLE_: return make_error_condition( invalid_argument );
    case ERROR_INVALID_NAME_: return make_error_condition( invalid_argument );
    case ERROR_LOCK_VIOLATION_: return make_error_condition( no_lock_available );
    case ERROR_LOCKED_: return make_error_condition( no_lock_available );
    case ERROR_NEGATIVE_SEEK_: return make_error_condition( invalid_argument );
    case ERROR_NOACCESS_: return make_error_condition( permission_denied );
    case ERROR_NOT_ENOUGH_MEMORY_: return make_error_condition( not_enough_memory );
    case ERROR_NOT_READY_: return make_error_condition( resource_unavailable_try_again );
    case ERROR_NOT_SAME_DEVICE_: return make_error_condition( cross_device_link );
    case ERROR_OPEN_FAILED_: return make_error_condition( io_error );
    case ERROR_OPEN_FILES_: return make_error_condition( device_or_resource_busy );
    case ERROR_OPERATION_ABORTED_: return make_error_condition( operation_canceled );
    case ERROR_OUTOFMEMORY_: return make_error_condition( not_enough_memory );
    case ERROR_PATH_NOT_FOUND_: return make_error_condition( no_such_file_or_directory );
    case ERROR_READ_FAULT_: return make_error_condition( io_error );
    case ERROR_RETRY_: return make_error_condition( resource_unavailable_try_again );
    case ERROR_SEEK_: return make_error_condition( io_error );
    case ERROR_SHARING_VIOLATION_: return make_error_condition( permission_denied );
    case ERROR_TOO_MANY_OPEN_FILES_: return make_error_condition( too_many_files_open );
    case ERROR_WRITE_FAULT_: return make_error_condition( io_error );
    case ERROR_WRITE_PROTECT_: return make_error_condition( permission_denied );
    case WSAEACCES_: return make_error_condition( permission_denied );
    case WSAEADDRINUSE_: return make_error_condition( address_in_use );
    case WSAEADDRNOTAVAIL_: return make_error_condition( address_not_available );
    case WSAEAFNOSUPPORT_: return make_error_condition( address_family_not_supported );
    case WSAEALREADY_: return make_error_condition( connection_already_in_progress );
    case WSAEBADF_: return make_error_condition( bad_file_descriptor );
    case WSAECONNABORTED_: return make_error_condition( connection_aborted );
    case WSAECONNREFUSED_: return make_error_condition( connection_refused );
    case WSAECONNRESET_: return make_error_condition( connection_reset );
    case WSAEDESTADDRREQ_: return make_error_condition( destination_address_required );
    case WSAEFAULT_: return make_error_condition( bad_address );
    case WSAEHOSTUNREACH_: return make_error_condition( host_unreachable );
    case WSAEINPROGRESS_: return make_error_condition( operation_in_progress );
    case WSAEINTR_: return make_error_condition( interrupted );
    case WSAEINVAL_: return make_error_condition( invalid_argument );
    case WSAEISCONN_: return make_error_condition( already_connected );
    case WSAEMFILE_: return make_error_condition( too_many_files_open );
    case WSAEMSGSIZE_: return make_error_condition( message_size );
    case WSAENAMETOOLONG_: return make_error_condition( filename_too_long );
    case WSAENETDOWN_: return make_error_condition( network_down );
    case WSAENETRESET_: return make_error_condition( network_reset );
    case WSAENETUNREACH_: return make_error_condition( network_unreachable );
    case WSAENOBUFS_: return make_error_condition( no_buffer_space );
    case WSAENOPROTOOPT_: return make_error_condition( no_protocol_option );
    case WSAENOTCONN_: return make_error_condition( not_connected );
    case WSAENOTSOCK_: return make_error_condition( not_a_socket );
    case WSAEOPNOTSUPP_: return make_error_condition( operation_not_supported );
    case WSAEPROTONOSUPPORT_: return make_error_condition( protocol_not_supported );
    case WSAEPROTOTYPE_: return make_error_condition( wrong_protocol_type );
    case WSAETIMEDOUT_: return make_error_condition( timed_out );
    case WSAEWOULDBLOCK_: return make_error_condition( operation_would_block );

    default: return error_condition( ev, system_category() );
    }
}

} // namespace detail

} // namespace system

} // namespace boost
