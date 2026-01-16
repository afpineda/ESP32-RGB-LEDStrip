/**
 * @file LedMatrixDefTest.cpp
 *
 * @author Ángel Fernández Pineda. Madrid. Spain. 2025.
 * @brief Test of LED matrix definitions
 *
 * @date 2026-01-16
 *
 * @copyright Under EUPL 1.2 license
 */

//-------------------------------------------------------------------
// Imports
//-------------------------------------------------------------------

#include "PixelMatrix.hpp"
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

//-------------------------------------------------------------------
// Globals
//-------------------------------------------------------------------

#define ROWS 3
#define COLUMNS 3

static const LedMatrixDefinition test_case[] =
    {
        // 0: top-left, rows, serpentine
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::top_left,
            .arrangement = LedMatrixArrangement::rows,
            .wiring = LedMatrixWiring::serpentine,
        },
        // 1: top-left, rows, linear
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::top_left,
            .arrangement = LedMatrixArrangement::rows,
            .wiring = LedMatrixWiring::linear,
        },
        // 2: top-right, rows, serpentine (zig-zag)
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::top_right,
            .arrangement = LedMatrixArrangement::rows,
            .wiring = LedMatrixWiring::zig_zag,
        },
        // 3: top-right, rows, linear (progressive)
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::top_right,
            .arrangement = LedMatrixArrangement::rows,
            .wiring = LedMatrixWiring::progressive,
        },
        // 4: bottom-left, rows, serpentine
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::bottom_left,
            .arrangement = LedMatrixArrangement::rows,
            .wiring = LedMatrixWiring::serpentine,
        },
        // 5: bottom-left, rows, linear
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::bottom_left,
            .arrangement = LedMatrixArrangement::rows,
            .wiring = LedMatrixWiring::linear,
        },
        // 6: bottom-right, rows, serpentine
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::bottom_right,
            .arrangement = LedMatrixArrangement::rows,
            .wiring = LedMatrixWiring::serpentine,
        },
        // 7: bottom-right, rows, linear
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::bottom_right,
            .arrangement = LedMatrixArrangement::rows,
            .wiring = LedMatrixWiring::linear,
        },
        // 8: top-left, columns, serpentine
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::top_left,
            .arrangement = LedMatrixArrangement::columns,
            .wiring = LedMatrixWiring::serpentine,
        },
        // 9: top-left, columns, linear
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::top_left,
            .arrangement = LedMatrixArrangement::columns,
            .wiring = LedMatrixWiring::linear,
        },
        // 10: top-right, columns, serpentine
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::top_right,
            .arrangement = LedMatrixArrangement::columns,
            .wiring = LedMatrixWiring::serpentine,
        },
        // 11: top-right, columns, linear
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::top_right,
            .arrangement = LedMatrixArrangement::columns,
            .wiring = LedMatrixWiring::linear,
        },
        // 12: bottom-left, columns, serpentine
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::bottom_left,
            .arrangement = LedMatrixArrangement::columns,
            .wiring = LedMatrixWiring::serpentine,
        },
        // 13: bottom-left, columns, linear
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::bottom_left,
            .arrangement = LedMatrixArrangement::columns,
            .wiring = LedMatrixWiring::linear,
        },
        // 14: bottom-right, columns, serpentine
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::bottom_right,
            .arrangement = LedMatrixArrangement::columns,
            .wiring = LedMatrixWiring::serpentine,
        },
        // 15: bottom-right, columns, linear
        {
            .row_count = 3,
            .column_count = 3,
            .first_pixel = LedMatrixFirstPixel::bottom_right,
            .arrangement = LedMatrixArrangement::columns,
            .wiring = LedMatrixWiring::linear,
        },
};

#define TEST_CASE_COUNT (sizeof(test_case) / sizeof(LedMatrixDefinition))

// Note:
// PixelMatrix is used here as a container of numbers
static const PixelMatrix expected[] =
    {
        // 0: top-left, rows, serpentine
        PixelMatrix({
            {0, 1, 2},
            {5, 4, 3},
            {6, 7, 8},
        }),
        // 1: top-left, rows, linear
        PixelMatrix({
            {0, 1, 2},
            {3, 4, 5},
            {6, 7, 8},
        }),
        // 2: top-right, rows, serpentine
        PixelMatrix({
            {2, 1, 0},
            {3, 4, 5},
            {8, 7, 6},
        }),
        // 3: top-right, rows, linear
        PixelMatrix({
            {2, 1, 0},
            {5, 4, 3},
            {8, 7, 6},
        }),
        // 4: bottom-left, rows, serpentine
        PixelMatrix({
            {6, 7, 8},
            {5, 4, 3},
            {0, 1, 2},
        }),
        // 5: bottom-left, rows, linear
        PixelMatrix({
            {6, 7, 8},
            {3, 4, 5},
            {0, 1, 2},
        }),
        // 6: bottom-right, rows, serpentine
        PixelMatrix({
            {8, 7, 6},
            {3, 4, 5},
            {2, 1, 0},
        }),
        // 7: bottom-right, rows, linear
        PixelMatrix({
            {8, 7, 6},
            {5, 4, 3},
            {2, 1, 0},
        }),
        // 8: top-left, columns, serpentine
        PixelMatrix({
            {0, 5, 6},
            {1, 4, 7},
            {2, 3, 8},
        }),
        // 9: top-left, columns, linear
        PixelMatrix({
            {0, 3, 6},
            {1, 4, 7},
            {2, 5, 8},
        }),
        // 10: top-right, columns, serpentine
        PixelMatrix({
            {6, 5, 0},
            {7, 4, 1},
            {8, 3, 2},
        }),
        // 11: top-right, columns, linear
        PixelMatrix({
            {6, 3, 0},
            {7, 4, 1},
            {8, 5, 2},
        }),
        // 12: bottom-left, columns, serpentine
        PixelMatrix({
            {2, 3, 8},
            {1, 4, 7},
            {0, 5, 6},
        }),
        // 13: bottom-left, columns, linear
        PixelMatrix({
            {2, 5, 8},
            {1, 4, 7},
            {0, 3, 6},
        }),
        // 14: bottom-right, columns, serpentine
        PixelMatrix({
            {8, 3, 2},
            {7, 4, 1},
            {6, 5, 0},
        }),
        // 15: bottom-right, columns, linear
        PixelMatrix({
            {8, 5, 2},
            {7, 4, 1},
            {6, 3, 0},
        }),
};

static_assert(
    (sizeof(expected) / sizeof(PixelMatrix)) == TEST_CASE_COUNT,
    "Result array does not match the size of the test case array");

#define CANONICAL_CASE 1

//-------------------------------------------------------------------
// Auxiliary
//-------------------------------------------------------------------

PixelMatrix build(const LedMatrixDefinition &source)
{
    PixelMatrix result(ROWS, COLUMNS);
    for (size_t i = 0; i < (ROWS * COLUMNS); i++)
    {
        size_t row;
        size_t col;
        source.indexToCoordinates(i, row, col);
        assert(row < ROWS);
        assert(col < COLUMNS);
        result.at(row, col) = i;
    }
    return result;
}

void print(const PixelMatrix &mtx)
{
    PixelMatrix::size_type col = 1;
    for (auto pixel : mtx)
    {
        cout << (unsigned int)pixel << " ";
        if (col >= mtx.column_count())
        {
            col = 1;
            cout << endl;
        }
        else
            col++;
    }
    cout << endl;
}

//-------------------------------------------------------------------
// Test groups
//-------------------------------------------------------------------

void test1()
{
    for (int case_index = 0; case_index < TEST_CASE_COUNT; case_index++)
    {
        cout << "- Index -> coordinates test case ";
        cout << case_index << " -" << endl;
        PixelMatrix result = build(test_case[case_index]);
        if (expected[case_index] != result)
        {
            cout << "Failure. Expected:" << endl;
            print(expected[case_index]);
            cout << "Got:" << endl;
            print(result);
            assert(expected[case_index] == result);
        }
    }
}

void test2()
{
    for (int case_index = 0; case_index < TEST_CASE_COUNT; case_index++)
    {
        cout << "- Coordinates -> index test case ";
        cout << case_index << " -" << endl;
        for (size_t row = 0; row < 3; row++)
        {
            for (size_t col = 0; col < 3; col++)
            {
                // cout << "Note. Row: " << row;
                // cout << " Column: " << col << endl;
                size_t expected_pixel_index =
                    (unsigned int)expected[case_index].at(row, col);
                size_t actual_pixel_index =
                    test_case[case_index].coordinatesToIndex(row, col);
                if (expected_pixel_index != actual_pixel_index)
                {
                    cout << "Failure. Row: " << row;
                    cout << " Column: " << col << endl;
                    cout << "Expected: " << expected_pixel_index << endl;
                    cout << "Found: " << actual_pixel_index << endl;
                    assert(expected_pixel_index == actual_pixel_index);
                }
            }
        }
    }
}

void test3()
{
    for (int case_index = 0; case_index < TEST_CASE_COUNT; case_index++)
    {
        cout << "- Index -> canonical index test case ";
        cout << case_index << " -" << endl;

        PixelMatrix result(ROWS, COLUMNS);
        for (size_t pixel_index = 0;
             pixel_index < (ROWS * COLUMNS);
             pixel_index++)
        {
            size_t row, col;
            test_case[case_index].indexToCoordinates(pixel_index, row, col);
            result.at(row, col) =
                test_case[case_index].canonicalIndex(pixel_index);
        }
        assert(result == expected[CANONICAL_CASE]);
    }
}

//-------------------------------------------------------------------
// MAIN
//-------------------------------------------------------------------

int main()
{
    test1();
    test2();
    test3();
    return 0;
}
