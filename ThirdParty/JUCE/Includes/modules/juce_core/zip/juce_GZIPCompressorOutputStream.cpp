/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-8-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

class GZIPCompressorOutputStream::GZIPCompressorHelper
{
public:
    GZIPCompressorHelper (int compressionLevel, int windowBits)
        : compLevel ((compressionLevel < 0 || compressionLevel > 9) ? -1 : compressionLevel)
    {
        using namespace zlibNamespace;
        zerostruct (stream);

        streamIsValid = (deflateInit2 (&stream, compLevel, Z_DEFLATED,
                                       windowBits != 0 ? windowBits : MAX_WBITS,
                                       8, strategy) == Z_OK);
    }

    ~GZIPCompressorHelper()
    {
        if (streamIsValid)
            zlibNamespace::deflateEnd (&stream);
    }

    bool write (const uint8* data, size_t dataSize, OutputStream& out)
    {
        // When you call flush() on a gzip stream, the stream is closed, and you can
        // no longer continue to write data to it!
        jassert (! finished);

        while (dataSize > 0)
            if (! doNextBlock (data, dataSize, out, Z_NO_FLUSH))
                return false;

        return true;
    }

    void finish (OutputStream& out)
    {
        const uint8* data = nullptr;
        size_t dataSize = 0;

        while (! finished)
            doNextBlock (data, dataSize, out, Z_FINISH);
    }

private:
    enum { strategy = 0 };

    zlibNamespace::z_stream stream;
    const int compLevel;
    bool isFirstDeflate = true, streamIsValid = false, finished = false;
    zlibNamespace::Bytef buffer[32768];

    bool doNextBlock (const uint8*& data, size_t& dataSize, OutputStream& out, const int flushMode)
    {
        using namespace zlibNamespace;

        if (streamIsValid)
        {
            stream.next_in   = const_cast<uint8*> (data);
            stream.next_out  = buffer;
            stream.avail_in  = (z_uInt) dataSize;
            stream.avail_out = (z_uInt) sizeof (buffer);

            auto result = isFirstDeflate ? deflateParams (&stream, compLevel, strategy)
                                         : deflate (&stream, flushMode);
            isFirstDeflate = false;

            switch (result)
            {
                case Z_STREAM_END:
                    finished = true;
                    JUCE_FALLTHROUGH
                case Z_OK:
                {
                    data += dataSize - stream.avail_in;
                    dataSize = stream.avail_in;
                    auto bytesDone = (ssize_t) sizeof (buffer) - (ssize_t) stream.avail_out;
                    return bytesDone <= 0 || out.write (buffer, (size_t) bytesDone);
                }

                default:
                    break;
            }
        }

        return false;
    }

    JUCE_DECLARE_NON_COPYABLE (GZIPCompressorHelper)
};

//==============================================================================
GZIPCompressorOutputStream::GZIPCompressorOutputStream (OutputStream& s, int compressionLevel, int windowBits)
   : GZIPCompressorOutputStream (&s, compressionLevel, false, windowBits)
{
}

GZIPCompressorOutputStream::GZIPCompressorOutputStream (OutputStream* out, int compressionLevel, bool deleteDestStream, int windowBits)
   : destStream (out, deleteDestStream),
     helper (new GZIPCompressorHelper (compressionLevel, windowBits))
{
    jassert (out != nullptr);
}

GZIPCompressorOutputStream::~GZIPCompressorOutputStream()
{
    flush();
}

void GZIPCompressorOutputStream::flush()
{
    helper->finish (*destStream);
    destStream->flush();
}

bool GZIPCompressorOutputStream::write (const void* destBuffer, size_t howMany)
{
    jassert (destBuffer != nullptr && (ssize_t) howMany >= 0);

    return helper->write (static_cast<const uint8*> (destBuffer), howMany, *destStream);
}

int64 GZIPCompressorOutputStream::getPosition()
{
    return destStream->getPosition();
}

bool GZIPCompressorOutputStream::setPosition (int64 /*newPosition*/)
{
    jassertfalse; // can't do it!
    return false;
}


//==============================================================================
//==============================================================================
#if JUCE_UNIT_TESTS

struct GZIPTests final : public UnitTest
{
    GZIPTests()
        : UnitTest ("GZIP", UnitTestCategories::compression)
    {}

    void runTest() override
    {
        beginTest ("GZIP");
        Random rng = getRandom();

        for (int i = 100; --i >= 0;)
        {
            MemoryOutputStream original, compressed, uncompressed;

            {
                GZIPCompressorOutputStream zipper (compressed, rng.nextInt (10));

                for (int j = rng.nextInt (100); --j >= 0;)
                {
                    MemoryBlock data ((unsigned int) (rng.nextInt (2000) + 1));

                    for (int k = (int) data.getSize(); --k >= 0;)
                        data[k] = (char) rng.nextInt (255);

                    original << data;
                    zipper   << data;
                }
            }

            {
                MemoryInputStream compressedInput (compressed.getData(), compressed.getDataSize(), false);
                GZIPDecompressorInputStream unzipper (compressedInput);

                uncompressed << unzipper;
            }

            expectEquals ((int) uncompressed.getDataSize(),
                          (int) original.getDataSize());

            if (original.getDataSize() == uncompressed.getDataSize())
                expect (memcmp (uncompressed.getData(),
                                original.getData(),
                                original.getDataSize()) == 0);
        }
    }
};

static GZIPTests gzipTests;

#endif

} // namespace juce
