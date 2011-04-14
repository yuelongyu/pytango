/*******************************************************************************

   This file is part of PyTango, a python binding for Tango

   http://www.tango-controls.org/static/PyTango/latest/doc/html/index.html

   Copyright 2011 CELLS / ALBA Synchrotron, Bellaterra, Spain
   
   PyTango is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   PyTango is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.
  
   You should have received a copy of the GNU Lesser General Public License
   along with PyTango.  If not, see <http://www.gnu.org/licenses/>.
   
*******************************************************************************/

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/copy_non_const_reference.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <tango/tango.h>

#include "pytgutils.h"
#include "device_attribute.h"

namespace PyGroupAttrReply
{
    using namespace boost::python;
    object get_data(Tango::GroupAttrReply& self, PyTango::ExtractAs extract_as)
    {
        // Usually we pass a device_proxy to "convert_to_python" in order to
        // get the data_format of the DeviceAttribute for Tango versions
        // older than 7.0. However, GroupAttrReply has no device_proxy to use!
        // So, we are using update_data_format() in:
        //       GroupElement::read_attribute_reply/read_attributes_reply
        return PyDeviceAttribute::convert_to_python(
                new Tango::DeviceAttribute(self.get_data()), extract_as );
    }
}


void export_group_reply()
{
    using namespace boost::python;

    class_<Tango::GroupReply> GroupReply("GroupReply", "", no_init);
    GroupReply
//         .def(init<>())
//         .def(init<const Tango::GroupReply &>())
//         .def(init<const std::string, const std::string, bool>()) /// @todo args?
//         .def(init<const std::string, const std::string, const Tango::DevFailed&, bool>())
        .def("has_failed", &Tango::GroupReply::has_failed)
        .def("group_element_enabled", &Tango::GroupReply::group_element_enabled)
        .def("dev_name", &Tango::GroupReply::dev_name, return_value_policy<copy_const_reference>())
        .def("obj_name", &Tango::GroupReply::obj_name, return_value_policy<copy_const_reference>())
        .def("get_err_stack", &Tango::GroupReply::get_err_stack, return_value_policy<copy_const_reference>())
    ;

    
    class_<Tango::GroupCmdReply, bases<Tango::GroupReply> > GroupCmdReply(
        "GroupCmdReply",
        no_init)
    ;
    GroupCmdReply
        .def("get_data_raw", &Tango::GroupCmdReply::get_data, return_internal_reference<1>() )
    ;

    
    class_<Tango::GroupAttrReply, bases<Tango::GroupReply> > GroupAttrReply(
        "GroupAttrReply",
        no_init)
    ;
    GroupAttrReply
            .def("__get_data",
                &PyGroupAttrReply::get_data,
                (   arg_("self"),
                    arg_("extract_as")=PyTango::ExtractAsNumpy) )
    ;
}