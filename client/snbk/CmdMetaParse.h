/*
 * Copyright (c) 2026 SUSE LLC
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


#ifndef SNAPPER_CMD_METAPARSE_H
#define SNAPPER_CMD_METAPARSE_H


#include <snapper/Enum.h>
#include <snapper/Snapshot.h>

#include "Shell.h"


namespace snapper
{
    using std::string;


    class SnapshotMeta
    {
    public:

	enum class State
	{
	    /**
	     * Errors occurred while processing the content of `info.xml`. Either the file
	     * does not exist or some attributes are missing. The state of `SnapshotMeta`
	     * is invalid.
	     */
	    INVALID,

	    /**
	     * The content of `info.xml` has been successfully processed. The state of
	     * `SnapshotMeta` is valid.
	     */
	    VALID
	};

	State state = State::INVALID;

	time_t date = 0;
	SnapshotType type = SnapshotType::SINGLE;
	unsigned int pre_num = 0;
	string cleanup;
	map<string, string> userdata;
    };


    template <> struct EnumInfo<SnapshotMeta::State>
    {
	static const vector<string> names;
    };


    class CmdMetaParse
    {
    public:

	CmdMetaParse(const Shell& shell, const string& snapshot_dir,
	             const string& cat_bin);

	string get_checksum() const;
	SnapshotMeta get_meta() const;

	friend std::ostream& operator<<(std::ostream& s,
	                                const CmdMetaParse& cmd_metaparse);

    private:

	const string path;

	string content;

    };

} // namespace snapper


#endif
