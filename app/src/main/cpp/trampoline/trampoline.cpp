//
// Created by SwiftGan on 2019/1/17.
//

#ifndef SANDHOOK_TRAMPOLINE_CPP
#define SANDHOOK_TRAMPOLINE_CPP

#include "trampoline.h"

namespace SandHook {

    class DirectJumpTrampoline : public Trampoline {
    public:

        DirectJumpTrampoline() : Trampoline::Trampoline() {}

        void setJumpTarget(Code target) {
            codeCopy(reinterpret_cast<Code>(&target), OFFSET_JUMP_ADDR_TARGET, BYTE_POINT);
        }

    protected:
        Size codeLength() override {
            return SIZE_DIRECT_JUMP_TRAMPOLINE;
        }

        Code templateCode() override {
            return reinterpret_cast<Code>(DIRECT_JUMP_TRAMPOLINE);
        }
    };

    class ReplacementHookTrampoline : public Trampoline {
    public:

        void setHookMethod(Code hookMethod) {
            codeCopy(reinterpret_cast<Code>(&hookMethod), OFFSET_REPLACEMENT_ADDR_ART_METHOD, BYTE_POINT);
        }

        void setEntryCodeOffset(Size offSet) {
            codeCopy(reinterpret_cast<Code>(&offSet), OFFSET_REPLACEMENT_OFFSET_ENTRY_CODE, BYTE_POINT);
        }

    protected:
        Size codeLength() override {
            return SIZE_REPLACEMENT_HOOK_TRAMPOLINE;
        }

        Code templateCode() override {
            return reinterpret_cast<Code>(REPLACEMENT_HOOK_TRAMPOLINE);
        }
    };

    class InlineHookTrampoline : public Trampoline {
    public:

        void setOriginMethod(Code originMethod) {
            codeCopy(reinterpret_cast<Code>(&originMethod), OFFSET_INLINE_ADDR_ORIGIN_METHOD, BYTE_POINT);
        }

        void setHookMethod(Code hookMethod) {
            codeCopy(reinterpret_cast<Code>(&hookMethod), OFFSET_INLINE_ADDR_HOOK_METHOD, BYTE_POINT);
        }

        void setEntryCodeOffset(Size offSet) {
            codeCopy(reinterpret_cast<Code>(&offSet), OFFSET_INLINE_OFFSET_ENTRY_CODE, BYTE_POINT);
            #if defined(__arm__)
            Code32Bit offset32;
            Code32Bit code32Bit;
            offset32.code = offSet;
            unsigned char offsetOP = isBigEnd() ? offset32.op.op4 : offset32.op.op1;
            code32Bit.code = *reinterpret_cast<uint32_t*>(((Size)getCode() + OFFSET_INLINE_OP_OFFSET_CODE));
            if (isBigEnd()) {
                code32Bit.op.op4 = offsetOP;
            } else {
                code32Bit.op.op1 = offsetOP;
            }
            #endif
        }

        void setOriginCode(Code originCode) {
            codeCopy(originCode, OFFSET_INLINE_ORIGIN_CODE, SIZE_DIRECT_JUMP_TRAMPOLINE);
        }

        Code getCallOriginCode() {
            return reinterpret_cast<Code>((Size)getCode() + OFFSET_INLINE_ORIGIN_CODE);
        }

    protected:
        Size codeLength() override {
            return SIZE_INLINE_HOOK_TRAMPOLINE;
        }

        Code templateCode() override {
            return reinterpret_cast<Code>(INLINE_HOOK_TRAMPOLINE);
        }
    };

    class CallOriginTrampoline : public Trampoline {
    public:

        void setOriginMethod(Code originMethod) {
            codeCopy(reinterpret_cast<Code>(&originMethod), OFFSET_CALL_ORIGIN_ART_METHOD, BYTE_POINT);
        }

        void setOriginCode(Code originCode) {
            codeCopy(reinterpret_cast<Code>(&originCode), OFFSET_CALL_ORIGIN_JUMP_ADDR, BYTE_POINT);
        }

    protected:
        Size codeLength() override {
            return SIZE_CALL_ORIGIN_TRAMPOLINE;
        }

        Code templateCode() override {
            return reinterpret_cast<Code>(CALL_ORIGIN_TRAMPOLINE);
        }
    };

}

#endif //SANDHOOK_TRAMPOLINE_CPP