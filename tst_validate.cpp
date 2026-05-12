#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include "validate.h"

using namespace testing;

static std::vector<std::vector<char>> make_field(const std::vector<std::string>& rows) {
    std::vector<std::vector<char>> field;
    field.reserve(rows.size());
    for (const auto& row : rows) {
        field.emplace_back(row.begin(), row.end());
    }
    return field;
}

static int count_errors(const std::vector<FieldError>& errors, ErrorKind kind, int len = -1) {
    int count = 0;
    for (const auto& e : errors) {
        if (e.kind == kind && (len == -1 || e.len == len))
            count++;
    }
    return count;
}

static bool has_error_at(const std::vector<FieldError>& errors, ErrorKind kind, Coord coord) {
    for (const auto& e : errors) {
        if (e.kind != kind) continue;
        for (const auto& c : e.coords) {
            if (c == coord) return true;
        }
    }
    return false;
}

TEST(validate, correct_field_no_errors) {
    auto field = make_field({
        "**00000000",
        "0000000000",
        "****000000",
        "0000000000",
        "***0000***",
        "0000000000",
        "**000000**",
        "0000000000",
        "*00*00*00*",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_TRUE(errors.empty());
}

TEST(validate, empty_field_reports_not_enough_for_all_sizes) {
    auto field = make_field({
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 1), 1);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 2), 1);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 3), 1);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 4), 1);
    EXPECT_EQ(count_errors(errors, TOO_MANY_SHIPS),      0);
    EXPECT_EQ(count_errors(errors, TOO_LONG_SHIP),       0);
    EXPECT_EQ(count_errors(errors, SHIPS_CONNECTED),     0);
}

TEST(validate, ships_touching_diagonally_exact_coords) {
    auto field = make_field({
        "*000000000",
        "0*00000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, SHIPS_CONNECTED), 2);
    EXPECT_TRUE(has_error_at(errors, SHIPS_CONNECTED, Coord{0, 0}));
    EXPECT_TRUE(has_error_at(errors, SHIPS_CONNECTED, Coord{1, 1}));
    EXPECT_EQ(count_errors(errors, TOO_LONG_SHIP),   0);
}



TEST(validate, five_cell_ship_reports_too_long_with_coords) {
    auto field = make_field({
        "*****00000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, TOO_LONG_SHIP), 1);
    for (int x = 0; x < 5; x++)
        EXPECT_TRUE(has_error_at(errors, TOO_LONG_SHIP, Coord{x, 0}));
    EXPECT_EQ(count_errors(errors, SHIPS_CONNECTED), 0);
}

TEST(validate, five_single_ships_exact_excess) {
    auto field = make_field({
        "*0*0*0*0*0",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, TOO_MANY_SHIPS, 1), 1);
    auto it = std::find_if(errors.begin(), errors.end(), [](const FieldError& e){
        return e.kind == TOO_MANY_SHIPS && e.len == 1;
    });
    ASSERT_NE(it, errors.end());
    EXPECT_EQ(it->n, 5);
    EXPECT_EQ(count_errors(errors, SHIPS_CONNECTED), 0);
    EXPECT_EQ(count_errors(errors, TOO_LONG_SHIP),   0);
}

TEST(validate, one_triple_ship_missing_one) {
    auto field = make_field({
        "****000000",
        "0000000000",
        "***0000000",
        "0000000000",
        "**0**000**",
        "0000000000",
        "*0*0*0*000",
        "0000000000",
        "0000000000",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 3), 1);
    auto it = std::find_if(errors.begin(), errors.end(), [](const FieldError& e){
        return e.kind == NOT_ENOUGH_SHIPS && e.len == 3;
    });
    ASSERT_NE(it, errors.end());
    EXPECT_EQ(it->n, 1);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 1), 0);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 2), 0);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 4), 0);
}

TEST(validate, vertical_four_ship_no_errors_exact) {
    auto field = make_field({
        "*000000000",
        "*000000000",
        "*000000000",
        "*000000000",
        "0000000000",
        "000***0000",
        "0000000000",
        "000***0000",
        "0000000000",
        "**0**0**0*"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, TOO_LONG_SHIP),   0);
    EXPECT_EQ(count_errors(errors, SHIPS_CONNECTED), 0);
}

TEST(validate, two_four_ships_exact_excess) {
    auto field = make_field({
        "****000***",
        "0000000000",
        "000000****",
        "0000000000",
        "***0000000",
        "0000000000",
        "**000000**",
        "0000000000",
        "*00*00*00*",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, TOO_MANY_SHIPS, 4), 1);
    auto it = std::find_if(errors.begin(), errors.end(), [](const FieldError& e){
        return e.kind == TOO_MANY_SHIPS && e.len == 4;
    });
    ASSERT_NE(it, errors.end());
    EXPECT_EQ(it->n, 2);
    EXPECT_EQ(count_errors(errors, TOO_LONG_SHIP),   0);
    EXPECT_EQ(count_errors(errors, SHIPS_CONNECTED), 0);
}

TEST(validate, ten_single_ships_all_error_kinds_present) {
    auto field = make_field({
        "*0*0*0*0*0",
        "0000000000",
        "*0*0*0*0*0",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, TOO_MANY_SHIPS,   1), 1);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 2), 1);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 3), 1);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS, 4), 1);
    auto it = std::find_if(errors.begin(), errors.end(), [](const FieldError& e){
        return e.kind == TOO_MANY_SHIPS && e.len == 1;
    });
    ASSERT_NE(it, errors.end());
    EXPECT_EQ(it->n, 10);
}

TEST(validate, three_ships_touching_in_chain_all_reported) {
    auto field = make_field({
        "*000000000",
        "0*00000000",
        "00*0000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, SHIPS_CONNECTED), 3);
    EXPECT_TRUE(has_error_at(errors, SHIPS_CONNECTED, Coord{0, 0}));
    EXPECT_TRUE(has_error_at(errors, SHIPS_CONNECTED, Coord{1, 1}));
    EXPECT_TRUE(has_error_at(errors, SHIPS_CONNECTED, Coord{2, 2}));
}

TEST(validate, vertical_ten_cell_ship_too_long_all_coords) {
    auto field = make_field({
        "*000000000",
        "*000000000",
        "*000000000",
        "*000000000",
        "*000000000",
        "*000000000",
        "*000000000",
        "*000000000",
        "*000000000",
        "*000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, TOO_LONG_SHIP), 1);
    for (int y = 0; y < 10; y++)
        EXPECT_TRUE(has_error_at(errors, TOO_LONG_SHIP, Coord{0, y}));
}

TEST(validate, no_extra_errors_on_correct_field) {
    auto field = make_field({
        "00000000**",
        "0000000000",
        "****000000",
        "0000000000",
        "***0000***",
        "0000000000",
        "**000000**",
        "0000000000",
        "*00*00*00*",
        "0000000000"
    });
    auto errors = validate_field(field);
    EXPECT_EQ(count_errors(errors, TOO_LONG_SHIP),   0);
    EXPECT_EQ(count_errors(errors, NOT_ENOUGH_SHIPS),0);
    EXPECT_EQ(count_errors(errors, TOO_MANY_SHIPS),  0);
    EXPECT_EQ(count_errors(errors, SHIPS_CONNECTED), 0);
}