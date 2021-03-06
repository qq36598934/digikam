/* ============================================================
 *
 * This file is a part of digiKam
 *
 * Date        : 2017-08-08
 * Description : Base functions for dnn module, can be used for face recognition, 
 *               all codes are ported from dlib library (http://dlib.net/)
 *
 * Copyright (C) 2006-2016 by Davis E. King <davis at dlib dot net>
 * Copyright (C) 2017      by Yingjie Liu <yingjiewudi at gmail dot com>
 * Copyright (C) 2017-2018 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef DLIB_DNn_UTILITIES_H_
#define DLIB_DNn_UTILITIES_H_

#include "core.h"

//namespace dlib
//{

// ----------------------------------------------------------------------------------------

    inline double log1pexp(double x)
    {
        using std::exp;
        using namespace std; // Do this instead of using std::log1p because some compilers
                             // error out otherwise (E.g. gcc 4.9 in cygwin)
        if (x <= -37)
            return exp(x);
        else if (-37 < x && x <= 18)
            return log1p(exp(x));
        else if (18 < x && x <= 33.3)
            return x + exp(-x);
        else
            return x;
    }
    
// ----------------------------------------------------------------------------------------

    inline void randomize_parameters (
        tensor& params,
        unsigned long num_inputs_and_outputs,
        drand& rnd
    )
    {
        for (auto& val : params)
        {
            // Draw a random number to initialize the layer according to formula (16)
            // from Understanding the difficulty of training deep feedforward neural
            // networks by Xavier Glorot and Yoshua Bengio.
            val = 2*rnd.get_random_float()-1;
            val *= std::sqrt(6.0/(num_inputs_and_outputs));
        }
    }

// ----------------------------------------------------------------------------------------

    namespace impl
    {
        class visitor_net_to_xml
        {
        public:

            visitor_net_to_xml(std::ostream& out_) : out(out_) {}

            template<typename input_layer_type>
            void operator()(size_t idx, const input_layer_type& l) 
            {
                out << "<layer idx='"<<idx<<"' type='input'>\n";
                to_xml(l,out);
                out << "</layer>\n";
            }

            template <typename T, typename U>
            void operator()(size_t idx, const add_loss_layer<T,U>& l) 
            {
                out << "<layer idx='"<<idx<<"' type='loss'>\n";
                to_xml(l.loss_details(),out);
                out << "</layer>\n";
            }

            template <typename T, typename U, typename E>
            void operator()(size_t idx, const add_layer<T,U,E>& l) 
            {
                out << "<layer idx='"<<idx<<"' type='comp'>\n";
                to_xml(l.layer_details(),out);
                out << "</layer>\n";
            }

            template <unsigned long ID, typename U, typename E>
            void operator()(size_t idx, const add_tag_layer<ID,U,E>& l) 
            {
                out << "<layer idx='"<<idx<<"' type='tag' id='"<<ID<<"'/>\n";
            }

            template <template<typename> class T, typename U>
            void operator()(size_t idx, const add_skip_layer<T,U>& l) 
            {
                out << "<layer idx='"<<idx<<"' type='skip' id='"<<(tag_id<T>::id)<<"'/>\n";
            }

        private:

            std::ostream& out;
        };
    }

    template <typename net_type>
    void net_to_xml (
        const net_type& net,
        std::ostream& out
    )
    {
        out << "<net>\n";
        visit_layers(net, impl::visitor_net_to_xml(out));
        out << "</net>\n";
    }

// ----------------------------------------------------------------------------------------

    namespace impl
    {

        class visitor_net_map_input_to_output
        {
        public:

            visitor_net_map_input_to_output(point& p_) : p(p_) {}

            point& p;

            template<typename layer_type>
            void operator()(size_t idx, const layer_type& net) 
            {
                p = net.layer_details().map_input_to_output(p);
            }
        };

        class visitor_net_map_output_to_input
        {
        public:
            visitor_net_map_output_to_input(point& p_) : p(p_) {}

            point& p;

            template<typename layer_type>
            void operator()(size_t idx, const layer_type& net) 
            {
                p = net.layer_details().map_output_to_input(p);
            }
        };
    }

    template <typename net_type>
    inline point input_tensor_to_output_tensor(
        const net_type& net,
        point p 
    )
    {
        impl::visitor_net_map_input_to_output temp(p);
        visit_layers_backwards_range<0,net_type::num_layers-1>(net, temp);
        return p;
    }

    template <typename net_type>
    inline point output_tensor_to_input_tensor(
        const net_type& net,
        point p  
    )
    {
        impl::visitor_net_map_output_to_input temp(p);
        visit_layers_range<0,net_type::num_layers-1>(net, temp);
        return p;
    }

// ----------------------------------------------------------------------------------------

//}

#endif // DLIB_DNn_UTILITIES_H_ 



