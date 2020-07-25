#include <gtest/gtest.h>

#include "../gtirb-builder/GtirbBuilder.h"
#include "../gtirb-decoder/DatalogLoader.h"
#include "../gtirb-decoder/DatalogProgram.h"

class DatalogLoaderTest : public ::testing::TestWithParam<const char *>
{
};

class TestDecoder : public GtirbDecoder
{
    void load(const gtirb::Module &M){};
    void populate(DatalogProgram &P){};
};

TEST_P(DatalogLoaderTest, loadElfGtirb)
{
    auto GTIRB = GtirbBuilder::read(GetParam());
    EXPECT_TRUE(GTIRB);

    gtirb::Module &Module = *(GTIRB->IR->modules().begin());

    std::vector<std::shared_ptr<GtirbDecoder>> Decoders = {
        std::make_shared<TestDecoder>(),
        std::make_shared<SectionDecoder>(),
    };
    DatalogLoader ElfLoader = DatalogLoader("test", Decoders);
    ElfLoader.load(Module);
}

INSTANTIATE_TEST_SUITE_P(GtirbDecoderTests, DatalogLoaderTest,
                         testing::Values("inputs/hello.x64.elf"));
