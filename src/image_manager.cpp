// This file is part of Heimer.
// Copyright (C) 2019 Jussi Lind <jussi.lind@iki.fi>
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

#include "image_manager.hpp"

#include "scene_items/node.hpp"

#include "simple_logger.hpp"

#include <algorithm>

ImageManager::ImageManager() = default;

ImageManager::ImageManager(const ImageManager & other)
  : QObject()
{
    m_images = other.m_images;
    m_count = other.m_count;
}

ImageManager & ImageManager::operator=(const ImageManager & other)
{
    if (this != &other) {
        m_images = other.m_images;
        m_count = other.m_count;
    }
    return *this;
}

size_t ImageManager::addImage(const Image & image)
{
    const auto id = ++m_count;
    m_images[id] = image;
    m_images[id].setId(id);

    juzzlin::L().debug() << "Adding new image, path=" << image.path() << ", id=" << id;

    return id;
}

void ImageManager::setImage(const Image & image)
{
    juzzlin::L().debug() << "Setting image, path=" << image.path() << ", id=" << image.id();

    if (!image.id()) {
        throw std::runtime_error("Image must have id > 0 !");
    }

    if (!image.image().size().height() || !image.image().size().width()) {
        juzzlin::L().warning() << "QImage size is zero!";
    }

    m_count = std::max(image.id(), m_count);
    m_images[image.id()] = image;
}

std::optional<Image> ImageManager::getImage(size_t id)
{
    if (m_images.count(id)) {
        return { m_images[id] };
    }
    return {};
}

void ImageManager::handleImageRequest(size_t id, NodeR node)
{
    if (const auto && imagePair = getImage(id); imagePair.has_value()) {
        juzzlin::L().debug() << "Applying image id=" << id << " to node " << node.index();
        node.applyImage(*imagePair);
    } else {
        juzzlin::L().warning() << "Cannot find image with id=" << id;
    }
}

ImageManager::ImageVector ImageManager::images() const
{
    ImageVector images;
    std::transform(std::begin(m_images), std::end(m_images), std::back_inserter(images),
                   [](auto && image) { return image.second; });
    return images;
}
