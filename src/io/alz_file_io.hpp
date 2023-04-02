// This file is part of Heimer.
// Copyright (C) 2018 Jussi Lind <jussi.lind@iki.fi>
//
// Heimer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// Heimer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Heimer. If not, see <http://www.gnu.org/licenses/>.

#ifndef ALZ_FILE_IO_HPP
#define ALZ_FILE_IO_HPP

#include "alz_file_io_version.hpp"
#include "file_io.hpp"
#include "types.hpp"

#include <memory>

class QThread;

namespace IO {

class AlzFileIOWorker;

class AlzFileIO : public FileIO
{
public:
    explicit AlzFileIO(AlzFormatVersion outputVersion = AlzFormatVersion::V2);

    ~AlzFileIO();

    void finish() override;

    MindMapDataU fromFile(QString path) const override;

    bool toFile(MindMapDataS mindMapData, QString path, bool async) const override;

    MindMapDataU fromXml(QString xml) const;

    QString toXml(MindMapDataS mindMapData) const;

private:
    std::unique_ptr<AlzFileIOWorker> m_worker;

    std::unique_ptr<QThread> m_workerThread;
};

} // namespace IO

#endif // ALZ_FILE_IO_HPP
