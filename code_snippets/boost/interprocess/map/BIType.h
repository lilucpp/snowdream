// Base - A C++ base library
// Copyright (c) 2023, Lu Li.  All rights reserved.
// https://github.com/lilucpp/base
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//   * Neither the name of Shuo Chen nor the names of other contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#pragma once

#include <functional>
#include <string>
#include <utility>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

namespace Helper {
namespace BI {
namespace bi = boost::interprocess;
typedef bi::allocator<bool, bi::managed_shared_memory::segment_manager> BoolAlloc;
typedef bi::allocator<char, bi::managed_shared_memory::segment_manager> CharAlloc;
typedef bi::allocator<wchar_t, bi::managed_shared_memory::segment_manager> WCharAlloc;
typedef bi::allocator<unsigned char, bi::managed_shared_memory::segment_manager> UCharAlloc;
typedef bi::allocator<int, bi::managed_shared_memory::segment_manager> IntAlloc;
typedef bi::allocator<unsigned int, bi::managed_shared_memory::segment_manager> UIntAlloc;
typedef bi::allocator<int32_t, bi::managed_shared_memory::segment_manager> Int32Alloc;
typedef bi::allocator<float, bi::managed_shared_memory::segment_manager> FloatAlloc;
typedef bi::allocator<double, bi::managed_shared_memory::segment_manager> DoubleAlloc;

typedef bi::basic_string<char, std::char_traits<char>, CharAlloc> BIString;
typedef bi::basic_string<wchar_t, std::char_traits<wchar_t>, WCharAlloc> BIWString;

typedef bi::allocator<BIString, bi::managed_shared_memory::segment_manager> StringAlloc;
typedef bi::allocator<BIWString, bi::managed_shared_memory::segment_manager> WStringAlloc;

// map key
typedef int KeyIntType;
typedef unsigned int KeyUIntType;
typedef BIString KeyStrType;
typedef BIWString KeyWStrType;

// map value
typedef int MappedIntType;
typedef unsigned int MappedUIntType;
typedef BIString MappedStrType;
typedef BIWString MappedWStrType;

// map pair
typedef std::pair<const KeyIntType, MappedIntType> IIPair;
typedef std::pair<const KeyIntType, MappedStrType> ISPair;
typedef std::pair<const KeyIntType, MappedWStrType> IWPair;
typedef std::pair<const KeyStrType, MappedIntType> SIPair;
typedef std::pair<const KeyStrType, MappedStrType> SSPair;
typedef std::pair<const KeyWStrType, MappedIntType> WIPair;
typedef std::pair<const KeyWStrType, MappedWStrType> WWPair;

typedef bi::allocator<IIPair, bi::managed_shared_memory::segment_manager> IIPairAlloc;
typedef bi::allocator<ISPair, bi::managed_shared_memory::segment_manager> ISPairAlloc;
typedef bi::allocator<IWPair, bi::managed_shared_memory::segment_manager> IWPairAlloc;
typedef bi::allocator<SIPair, bi::managed_shared_memory::segment_manager> SIPairAlloc;
typedef bi::allocator<SSPair, bi::managed_shared_memory::segment_manager> SSPairAlloc;
typedef bi::allocator<WIPair, bi::managed_shared_memory::segment_manager> WIPairAlloc;
typedef bi::allocator<WWPair, bi::managed_shared_memory::segment_manager> WWPairAlloc;

typedef bi::multimap<KeyIntType, MappedIntType, std::less<KeyIntType>, IIPairAlloc> IIMultimap;
typedef bi::multimap<KeyIntType, MappedStrType, std::less<KeyIntType>, ISPairAlloc> ISMultimap;
typedef bi::multimap<KeyIntType, MappedWStrType, std::less<KeyIntType>, IWPairAlloc> IWMultimap;
typedef bi::multimap<KeyStrType, MappedIntType, std::less<KeyStrType>, SIPairAlloc> SIMultimap;
typedef bi::multimap<KeyStrType, MappedStrType, std::less<KeyStrType>, SSPairAlloc> SSMultimap;
typedef bi::multimap<KeyWStrType, MappedIntType, std::less<KeyWStrType>, WIPairAlloc> WIMultimap;
typedef bi::multimap<KeyWStrType, MappedWStrType, std::less<KeyWStrType>, WWPairAlloc> WWMultimap;

typedef bi::map<KeyIntType, MappedIntType, std::less<KeyIntType>, IIPairAlloc> IIMap;
typedef bi::map<KeyIntType, MappedStrType, std::less<KeyIntType>, ISPairAlloc> ISMap;
typedef bi::map<KeyIntType, MappedWStrType, std::less<KeyIntType>, IWPairAlloc> IWMap;
typedef bi::map<KeyStrType, MappedIntType, std::less<KeyStrType>, SIPairAlloc> SIMap;
typedef bi::map<KeyStrType, MappedStrType, std::less<KeyStrType>, SSPairAlloc> SSMap;
typedef bi::map<KeyWStrType, MappedIntType, std::less<KeyWStrType>, WIPairAlloc> WIMap;
typedef bi::map<KeyWStrType, MappedWStrType, std::less<KeyWStrType>, WWPairAlloc> WWMap;

typedef bi::vector<int, IntAlloc> IVector;
typedef bi::vector<BIString, StringAlloc> SVector;

}  // namespace BI
}  // namespace Helper
