/**
 * @file RgbLedController.hpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Library for controlling LED strips
 *
 * @date 2025-10-21
 *
 * @copyright Under EUPL 1.2 License
 */

#pragma once

//------------------------------------------------------------------------------

#include "PixelDriver.hpp"
#include "PixelVector.hpp"
#include <vector> // For ::std::vector
#include <mutex>  // for ::std::mutex and ::std::lock_guard

class RgbGuard; // Forward declaration

//------------------------------------------------------------------------------

/**
 * @brief Abstract RGB LED controller
 *
 * @note An RGB LED controller is a piece of hardware
 *       that can display an array of RGB pixels.
 */
class RgbLedController
{
    friend class RgbGuard;

private:
    ::std::mutex acquireReleaseMutex;
    ::std::vector<const RgbGuard *> priorityQueue;
    const RgbGuard *prioritizedGuard = nullptr;

    /**
     * @brief Acquire prioritized access to the RGB LED controller
     *
     * @param guard Guard requiring access (non-null)
     */
    void acquire(const RgbGuard *guard);

    /**
     * @brief Release prioritized access to the RGB LED controller
     *
     * @param guard Guard releasing access (non-null)
     */
    void release(const RgbGuard *guard);

    /**
     * @brief Display pixels (all at once) if the guard has the priority
     *
     * @note If @p guard did not call acquire() previously, there is no effect.
     *
     * @note If another guard has higher priority, this call is ignored.
     *
     * @param pixels Pixel array
     * @param guard Guard having access (non-null)
     */
    void show(const PixelVector &pixels, const RgbGuard *guard);

public:
    /**
     * @brief Construct the RGB LED controller
     *
     */
    RgbLedController() noexcept {}

    virtual ~RgbLedController() {}
    RgbLedController(RgbLedController &&source);
    RgbLedController &operator=(RgbLedController &&source);
    RgbLedController(const RgbLedController &) = delete;
    RgbLedController &operator=(const RgbLedController &) = delete;

    /**
     * @brief Display pixels (all at once) ignoring display priority
     *
     * @note Thread-safe. Ignores any display guard.
     *
     * @param pixels Pixel vector
     */
    virtual void show(const PixelVector &pixels) = 0;

}; // RgbLedController

//------------------------------------------------------------------------------

/**
 * @brief Guard for prioritized display on any RGB Led controller
 *
 * @note This guard provides prioritized access to the controller
 *       without blocking any caller thread.
 *       If there are two guards with the same display priority,
 *       the one created first has priority.
 *       The guard with the highest priority can display pixels
 *       while the others are simply ignored.
 *
 */
class RgbGuard
{
private:
    RgbLedController *controller = nullptr;
    uint8_t _priority;

public:
    /**
     * @brief Display pixels (all at once)
     *
     * @note Thread-safe
     *
     * @param pixels Pixel array
     */
    void show(const PixelVector &pixels) const;

    /**
     * @brief Acquire a display priority guard
     *
     * @param controller Controller performing the actual display
     * @param priority Desired display priority.
     *                 An higher value means higher priority.
     */
    explicit RgbGuard(RgbLedController &controller, uint8_t priority) noexcept;

    /**
     * @brief Release the display priority guard
     *
     */
    ~RgbGuard() noexcept;

    /**
     * @brief Reacquire the display guard using another priority
     *
     * @param priority Desired display priority.
     *                 An higher value means higher priority.
     */
    void reacquire(uint8_t priority) noexcept;

    /**
     * @brief Get the display priority
     *
     * @return uint8_t Display priority
     */
    inline uint8_t priority() const noexcept { return _priority; }

    bool operator<(const RgbGuard &other) const noexcept
    {
        return _priority < other._priority;
    }

    RgbGuard(const RgbGuard &) = delete;
    RgbGuard &operator=(const RgbGuard &) = delete;
    RgbGuard(RgbGuard &&) = delete;
    RgbGuard &operator=(RgbGuard &&) = delete;
}; // RgbGuard
