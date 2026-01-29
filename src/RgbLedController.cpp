/**
 * @file RgbLedController.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2025-10-21
 *
 * @copyright Under EUPL 1.2 License
 */

//------------------------------------------------------------------------------
// Imports and globals
//------------------------------------------------------------------------------

#include <algorithm> // Required by priority queues
#include <cassert>   // For assert()
#include <memory>    // for ::std::addresof()
#include "RgbLedController.hpp"

//------------------------------------------------------------------------------
// RgbLedController
//------------------------------------------------------------------------------

void RgbLedController::acquire(const RgbGuard *guard)
{
    assert(guard);
    ::std::lock_guard<::std::mutex> lock(acquireReleaseMutex);
    priorityQueue.push_back(guard);
    if (!prioritizedGuard || (guard->priority() > prioritizedGuard->priority()))
        prioritizedGuard = guard;
}

void RgbLedController::release(const RgbGuard *guard)
{
    assert(guard);
    ::std::lock_guard<::std::mutex> lock(acquireReleaseMutex);
    auto iterator =
        ::std::find(priorityQueue.begin(), priorityQueue.end(), guard);
    assert(
        iterator != priorityQueue.end() &&
        "Led strip released but not acquired previously");
    priorityQueue.erase(iterator);
    if (priorityQueue.empty())
        prioritizedGuard = nullptr;
    else
        prioritizedGuard = *::std::max_element(
            priorityQueue.begin(),
            priorityQueue.end(),
            [](const RgbGuard *a, const RgbGuard *b)
            {
                return a->priority() < b->priority();
            });
}

bool RgbLedController::show(const PixelVector &pixels, const RgbGuard *guard)
{
    assert(guard);
    if (guard == prioritizedGuard)
    {
        show(pixels);
        return true;
    } else
        return false;
}

RgbLedController::RgbLedController(RgbLedController &&source)
{
    priorityQueue = ::std::move(source.priorityQueue);
    prioritizedGuard = source.prioritizedGuard;
}

RgbLedController &RgbLedController::operator=(RgbLedController &&source)
{
    priorityQueue = ::std::move(source.priorityQueue);
    prioritizedGuard = source.prioritizedGuard;
    return *this;
}

//------------------------------------------------------------------------------
// RgbGuard
//------------------------------------------------------------------------------

RgbGuard::RgbGuard(RgbLedController &controller, uint8_t priority) noexcept
{
    this->_priority = priority;
    this->controller = ::std::addressof(controller);
    this->controller->acquire(this);
}

void RgbGuard::reacquire(uint8_t priority) noexcept
{
    if (_priority != priority)
    {
        controller->release(this);
        _priority = priority;
        controller->acquire(this);
    }
}

RgbGuard::~RgbGuard() noexcept
{
    controller->release(this);
}

bool RgbGuard::show(const PixelVector &pixels) const
{
    return controller->show(pixels, this);
}