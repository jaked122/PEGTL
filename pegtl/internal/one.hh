// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_ONE_HH
#define PEGTL_INTERNAL_ONE_HH

#include <utility>
#include <algorithm>

#include "bump_util.hh"
#include "skip_control.hh"
#include "result_on_found.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename Char >
      bool contains( const Char c, const std::initializer_list< Char > & l )
      {
         return std::find( l.begin(), l.end(), c ) != l.end();
      }

      template< result_on_found R, typename Peek, typename Peek::data_t ... Cs >
      struct one
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( ! in.empty() ) {
               if ( const auto t = Peek::peek( in ) ) {
                  if ( contains( t.data, { Cs ... } ) == bool( R ) ) {
                     bump< R, Input, typename Peek::data_t, Cs ... >( in, t.size );
                     return true;
                  }
               }
            }
            return false;
         }
      };

      template< result_on_found R, typename Peek, typename Peek::data_t C >
      struct one< R, Peek, C >
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< typename Input >
         static bool match( Input & in )
         {
            if ( ! in.empty() ) {
               if ( const auto t = Peek::peek( in ) ) {
                  if ( ( t.data == C ) == bool( R ) ) {
                     bump< R, Input, typename Peek::data_t, C >( in, t.size );
                     return true;
                  }
               }
            }
            return false;
         }
      };

      template< result_on_found R, typename Peek, typename Peek::data_t ... Cs >
      struct skip_control< one< R, Peek, Cs ... > > : std::true_type {};

   } // namespace internal

} // namespace pegtl

#endif
