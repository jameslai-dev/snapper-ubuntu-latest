/*
 * Copyright (c) [2011-2014] Novell, Inc.
 * Copyright (c) 2020 SUSE LLC
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#include <regex>
#include <boost/algorithm/string.hpp>

#include "snapper/LvmUtils.h"
#include "snapper/SnapperDefines.h"


namespace snapper
{

    namespace LvmUtils
    {

	pair<string, string>
	split_device_name(const string& name)
	{
	    // Device mapper uses /dev/mapper/<vg_name>-<lv_name> format where
	    // hyphens in names are escaped as "--" and a single "-" separates VG/LV.
	    // Parse the name left-to-right, treating "--" as an escaped hyphen and the
	    // first standalone "-" as the VG/LV separator.

	    const string prefix = DEV_MAPPER_DIR "/";

	    if (!boost::starts_with(name, prefix))
		throw std::runtime_error("failed to split device name into volume group and "
					 "logical volume name");

	    const string basename = name.substr(prefix.length());
	    string vg_name, lv_name;
	    size_t i = 0;

	    while (i < basename.length())
	    {
		if (basename[i] == '-')
		{
		    if (i + 1 < basename.length() && basename[i + 1] == '-')
		    {
			vg_name += '-';
			i += 2;
		    }
		    else
		    {
			i++;
			break;
		    }
		}
		else
		{
		    vg_name += basename[i];
		    i++;
		}
	    }

	    while (i < basename.length())
	    {
		if (basename[i] == '-' && i + 1 < basename.length() && basename[i + 1] == '-')
		{
		    lv_name += '-';
		    i += 2;
		}
		else
		{
		    lv_name += basename[i];
		    i++;
		}
	    }

	    if (vg_name.empty() || lv_name.empty())
		throw std::runtime_error("failed to split device name into volume group and "
					 "logical volume name");

	    return make_pair(vg_name, lv_name);
	}

    }

}
