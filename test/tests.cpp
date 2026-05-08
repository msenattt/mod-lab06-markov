// Copyright 2024 <Copyright Owner>

#include <algorithm>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "textgen.h"

TEST(TextGeneratorTest, MakePrefixFromWords) {
    std::vector<std::string> words = {"жил", "старик", "со", "своею"};
    prefix pref = TextGenerator::makePrefix(words);
    EXPECT_EQ(pref.size(), 4);
    EXPECT_EQ(pref[0], "жил");
    EXPECT_EQ(pref[1], "старик");
    EXPECT_EQ(pref[2], "со");
    EXPECT_EQ(pref[3], "своею");
}

TEST(TextGeneratorTest, AddPrefixSuffixEntry) {
    TextGenerator gen(2);
    prefix pref = TextGenerator::makePrefix({"жил", "старик"});
    gen.addEntry(pref, "со");
    ASSERT_TRUE(gen.hasPrefix(pref));
    auto suffixes = gen.getSuffixes(pref);
    ASSERT_EQ(suffixes.size(), 1);
    EXPECT_EQ(suffixes[0], "со");
}

TEST(TextGeneratorTest, SingleSuffixRandomChoice) {
    TextGenerator gen(2);
    prefix pref = TextGenerator::makePrefix({"золотая", "рыбка"});
    gen.addEntry(pref, "голосом");
    std::vector<std::string> results;
    for (int i = 0; i < 5; i++) {
        results.push_back(gen.getGeneratedText(4));
    }
    for (const auto& text : results) {
        EXPECT_NE(text.find("голосом"), std::string::npos);
        EXPECT_FALSE(text.empty());
    }
}

TEST(TextGeneratorTest, MultipleSuffixRandomChoice) {
    TextGenerator gen(2);
    prefix pref = TextGenerator::makePrefix({"к", "синему"});
    gen.addEntry(pref, "морю");
    gen.addEntry(pref, "небу");
    gen.addEntry(pref, "озеру");
    int countMoru = 0, countNebu = 0, countOzeru = 0;
    for (int i = 0; i < 100; i++) {
        std::string text = gen.getGeneratedText(4);
        if (text.find("морю") != std::string::npos) countMoru++;
        if (text.find("небу") != std::string::npos) countNebu++;
        if (text.find("озеру") != std::string::npos) countOzeru++;
    }
    EXPECT_GT(countMoru, 0);
    EXPECT_GT(countNebu, 0);
    EXPECT_GT(countOzeru, 0);
    EXPECT_EQ(countMoru + countNebu + countOzeru, 100);
}

TEST(TextGeneratorTest, RandomChoiceWithDifferentProbabilities) {
    TextGenerator gen(2);
    prefix pref = TextGenerator::makePrefix({"у", "самого"});
    gen.addEntry(pref, "синего");
    gen.addEntry(pref, "синего");
    gen.addEntry(pref, "синего");
    gen.addEntry(pref, "красного");
    auto suffixes = gen.getSuffixes(pref);
    EXPECT_EQ(suffixes.size(), 4);
    int countSinego = 0, countKrasnogo = 0;
    for (int i = 0; i < 200; i++) {
        std::string text = gen.getGeneratedText(4);
        if (text.find("синего") != std::string::npos) countSinego++;
        if (text.find("красного") != std::string::npos) countKrasnogo++;
    }
    EXPECT_GT(countSinego, countKrasnogo);
    EXPECT_GT(countSinego, 100);
    EXPECT_GT(countKrasnogo, 0);
}

TEST(TextGeneratorTest, GeneratedTextLength) {
    TextGenerator gen(2);
    gen.addEntry(TextGenerator::makePrefix({"жил", "старик"}), "со");
    gen.addEntry(TextGenerator::makePrefix({"старик", "со"}), "своею");
    gen.addEntry(TextGenerator::makePrefix({"со", "своею"}), "старухой");
    gen.addEntry(TextGenerator::makePrefix({"своею", "старухой"}), "у");
    gen.addEntry(TextGenerator::makePrefix({"старухой", "у"}), "самого");
    gen.addEntry(TextGenerator::makePrefix({"у", "самого"}), "синего");
    gen.addEntry(TextGenerator::makePrefix({"самого", "синего"}), "моря");
    gen.addEntry(TextGenerator::makePrefix({"синего", "моря"}), "");

    auto countWords = [](const std::string& s) {
        if (s.empty()) return 0;
        int n = 1;
        for (char c : s) if (c == ' ') n++;
        return n;
    };
    std::string text5 = gen.getGeneratedText(5);
    std::string text7 = gen.getGeneratedText(7);
    std::string text10 = gen.getGeneratedText(10);
    EXPECT_LE(countWords(text5), 5);
    EXPECT_LE(countWords(text7), 7);
    EXPECT_LE(countWords(text10), 10);
    EXPECT_FALSE(text5.empty());
    EXPECT_FALSE(text7.empty());
    EXPECT_FALSE(text10.empty());
}

TEST(TextGeneratorTest, LoadFromString) {
    TextGenerator gen(2);
    gen.loadFromString("Жил старик со своею старухой У самого синего моря");
    EXPECT_GT(gen.getTableSize(), 0);
    EXPECT_TRUE(gen.hasPrefix(TextGenerator::makePrefix({"Жил", "старик"})));
    EXPECT_TRUE(gen.hasPrefix(TextGenerator::makePrefix({"синего", "моря"})));
}

TEST(TextGeneratorTest, EmptyTable) {
    TextGenerator gen(2);
    std::string text = gen.getGeneratedText(10);
    EXPECT_TRUE(text.empty());
    EXPECT_EQ(gen.getTableSize(), 0);
}

TEST(TextGeneratorTest, AddMultipleSuffixesToPrefix) {
    TextGenerator gen(2);
    prefix pref = TextGenerator::makePrefix({"в", "синее"});
    gen.addEntry(pref, "море");
    gen.addEntry(pref, "небо");
    gen.addEntry(pref, "озеро");
    auto suffixes = gen.getSuffixes(pref);
    EXPECT_EQ(suffixes.size(), 3);
    EXPECT_EQ(suffixes[0], "море");
    EXPECT_EQ(suffixes[1], "небо");
    EXPECT_EQ(suffixes[2], "озеро");
}

TEST(TextGeneratorTest, ManualTableGeneration) {
    TextGenerator gen(2);
    gen.addEntry(TextGenerator::makePrefix({"a", "b"}), "c");
    gen.addEntry(TextGenerator::makePrefix({"b", "c"}), "d");
    gen.addEntry(TextGenerator::makePrefix({"c", "d"}), "e");
    gen.addEntry(TextGenerator::makePrefix({"d", "e"}), "f");
    gen.addEntry(TextGenerator::makePrefix({"e", "f"}), "g");
    gen.addEntry(TextGenerator::makePrefix({"f", "g"}), "");
    std::string text = gen.getGeneratedText(10);
    EXPECT_FALSE(text.empty());
}
