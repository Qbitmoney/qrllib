// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.
#include <xmss-alt/algsxmss.h>
#include <xmssBasic.h>
#include <iostream>
#include "gtest/gtest.h"
#include <misc.h>

//FIXME: Unify with XmssFast tests

namespace {
#define XMSS_HEIGHT 4

    TEST(XmssBasicSHA2, Instantiation) {
        std::vector<unsigned char> seed(48, 0);

        XmssBasic xmss(seed, XMSS_HEIGHT, eHashFunction::SHA2);

        auto pk = xmss.getPK();
        auto sk = xmss.getSK();

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "seed:" << seed.size() << " bytes\n" << bin2hstr(seed, 48) << std::endl;
        std::cout << "pk  :" << pk.size() << " bytes\n" << bin2hstr(pk, 48) << std::endl;
        std::cout << "sk  :" << sk.size() << " bytes\n" << bin2hstr(sk, 48) << std::endl;

        std::cout << "descr:" << bin2hstr(xmss.getDescriptor()) << std::endl;
        std::cout << "addr :" << bin2hstr(xmss.getAddress()) << std::endl;

        EXPECT_EQ(seed, xmss.getSeed());
        EXPECT_EQ("0002", bin2hstr(xmss.getDescriptor()));
        EXPECT_EQ("000259d3414a5c481ef80c2608a1bf16c718b107b5d39619824cedf8f34bade455585f1d2a0c",
                  bin2hstr(xmss.getAddress()));
    }

    TEST(XmssBasicSHA2, SignatureLen) {
        std::vector<unsigned char> seed(48, 0);

        XmssBasic xmss4(seed, 4, eHashFunction::SHA2);
        EXPECT_EQ(2308, xmss4.getSignatureSize());

        XmssBasic xmss6(seed, 6, eHashFunction::SHA2);
        EXPECT_EQ(2372, xmss6.getSignatureSize());
    }

    TEST(XmssBasicSHA2, Sign) {
        std::vector<unsigned char> seed(48, 0);

        XmssBasic xmss(seed, XMSS_HEIGHT, eHashFunction::SHA2);

        std::string message = "This is a test message";
        std::vector<unsigned char> data(message.begin(), message.end());
        EXPECT_EQ(xmss.getIndex(), 0);

        auto signature = xmss.sign(data);

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "data       :" << data.size() << " bytes\n" << bin2hstr(data, 64) << std::endl;
        std::cout << "signature  :" << signature.size() << " bytes\n" << bin2hstr(signature, 64) << std::endl;
        EXPECT_EQ(xmss.getIndex(), 1);

        auto signature2 = xmss.sign(data);

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "data       :" << data.size() << " bytes\n" << bin2hstr(data, 64) << std::endl;
        std::cout << "signature  :" << signature.size() << " bytes\n" << bin2hstr(signature, 64) << std::endl;

        EXPECT_NE(bin2hstr(signature), bin2hstr(signature2));
        EXPECT_EQ(xmss.getIndex(), 2);
    }


    TEST(XmssBasicSHA2, Verify) {
        std::vector<unsigned char> seed;
        for(unsigned char i=0; i<48; i++)
            seed.push_back(i);

        XmssBasic xmss(seed, XMSS_HEIGHT, eHashFunction::SHA2);

        std::string message = "This is a test message";
        std::vector<unsigned char> data_ref(message.begin(), message.end());
        std::vector<unsigned char> data(message.begin(), message.end());

        auto pk = xmss.getPK();
        auto sk = xmss.getSK();
        std::cout << std::endl;
        std::cout << "seed:" << seed.size() << " bytes\n" << bin2hstr(seed, 32) << std::endl;
        std::cout << "pk  :" << pk.size() << " bytes\n" << bin2hstr(pk, 32) << std::endl;
        std::cout << "sk  :" << sk.size() << " bytes\n" << bin2hstr(sk, 32) << std::endl;

        auto signature = xmss.sign(data);

        EXPECT_EQ(data, data_ref);

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "data       :" << data.size() << " bytes\n" << bin2hstr(data, 64) << std::endl;
        std::cout << "signature  :" << signature.size() << " bytes\n" << bin2hstr(signature, 64) << std::endl;

        EXPECT_TRUE(XmssBasic::verify(data, signature, pk, eHashFunction::SHA2));

        signature[1] += 1;
        EXPECT_FALSE(XmssBasic::verify(data, signature, xmss.getPK()));
    }
}
