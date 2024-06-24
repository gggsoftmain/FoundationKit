/*
Copyright 2024 GGGSoft.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Ported and refactored from Java to C++

Java code written by k3d3
Source: https://github.com/k3d3/ed25519-java/blob/master/ed25519.java
Released to the public domain
*/

#include "Crypto/BigIntExtension.h"
#include "Math/BigInt.h"

namespace BigIntExtension
{
    typedef TBigInt<2048> TBigintEx;

    static int32 b = 256;
    static TBigintEx q("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFED");
    static TBigintEx qm2("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEB");
    static TBigintEx qp3("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0");
    static TBigintEx l("1000000000000000000000000000000014DEF9DEA2F79CD65812631A5CF5D3ED");
    static TBigintEx d("98412DFC9311D490018C7338BF8688861767FF8FF5B2BEBE27548A14B235EC8FEDA4");
    static TBigintEx I("2B8324804FC1DF0B2B4D00993DFBD7A72F431806AD2FE478C4EE1B274A0EA0B0");
    static TBigintEx By("6666666666666666666666666666666666666666666666666666666666666658");
    static TBigintEx Bx("216936D3CD6E53FEC0A4E231FDD6DC5C692CC7609525A7B2C9562D608F25D51A");
    static TBigintEx un("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    const TBigintEx Zero((int64)0);
    const TBigintEx One((int64)1);
    const TBigintEx Two((int64)2);
    const TBigintEx Eight((int64)8);

    TBigintEx Mod(TBigintEx num, TBigintEx modulo)
    {
        TBigintEx result;
        if (num.IsNegative())
        {
            num.Negate();
            result = num % modulo;
            result.Negate();
            num.Negate();
        }
        else
        {
            result = num % modulo;
        }
        
        return result < 0 ? modulo + result : result;
    }

    TBigintEx ExpMod(const TBigintEx& number, const TBigintEx& exponent, const TBigintEx& modulo)
    {
        if (exponent == Zero)
        {
            return One;
        }

        TBigintEx halfExponent = exponent / Two;
        TBigintEx t = ExpMod(number, halfExponent, modulo);
        t = Mod((t * t), modulo);

        if ((exponent.GetBits()[0] & 1) == 1)
        {
            t *= number;
            t = Mod(t, modulo);
        }

        return t;
    }

    static TBigintEx Inv(const TBigintEx& x)
    {
        return ExpMod(x, qm2, q);
    }

    static TBigintEx RecoverX(const TBigintEx& y)
    {
        TBigintEx y2 = y * y;
        TBigintEx xx = (y2 - 1) * Inv(d * y2 + 1);
        TBigintEx x = ExpMod(xx, qp3 / Eight, q);
        TBigintEx x2 = x * x;

        TBigintEx x2mxx = x2 - xx;

        if (Mod(x2mxx, q) != Zero)
        {
            x = Mod((x * I), q);
        }

        if ((x.GetBits()[0] & 1) == 1)
        {
            x = q - x;
        }

        return x;
    }

    static bool IsOnCurve(const TBigintEx& x, const TBigintEx& y)
    {
        TBigintEx xx = x * x;
        TBigintEx yy = y * y;
        TBigintEx dxxyy = d * yy * xx;
        TBigintEx yymxx = (yy - xx);
        TBigintEx yymxxmdxxyy = (yymxx - dxxyy);

        return Mod(yymxxmdxxyy - 1, q) == Zero;
    }
}


bool TBigIntExtension::IsOnCurve(const TArray<uint8>& key)
{
    if (BigIntExtension::d.IsNegative() == false)
    {
        BigIntExtension::d.Negate();
    }
    BigIntExtension::TBigintEx y = BigIntExtension::TBigintEx(key.GetData(), key.Num()) & BigIntExtension::un;
    BigIntExtension::TBigintEx x = BigIntExtension::RecoverX(y);

    return BigIntExtension::IsOnCurve(x, y);
}