//------------------------------------------------------------------- -*- cpp -*-
// Kunai-static-analyzer: library for doing analysis of dalvik files
// @author Farenain <kunai.static.analysis@gmail.com>
//
// @file InstructionLifter.cpp
#include "Lifter/MjolnIRLifter.hpp"
#include "Kunai/Exceptions/lifter_exception.hpp"

using namespace KUNAI::MjolnIR;

void Lifter::gen_instruction(KUNAI::DEX::Instruction *instr)
{
    switch (instr->get_instruction_type())
    {
    case KUNAI::DEX::dexinsttype_t::DEX_INSTRUCTION23X:
        gen_instruction(reinterpret_cast<KUNAI::DEX::Instruction23x *>(instr));
        break;
    case KUNAI::DEX::dexinsttype_t::DEX_INSTRUCTION12X:
        gen_instruction(reinterpret_cast<KUNAI::DEX::Instruction12x *>(instr));
        break;
    case KUNAI::DEX::dexinsttype_t::DEX_INSTRUCTION11X:
        gen_instruction(reinterpret_cast<KUNAI::DEX::Instruction11x *>(instr));
        break;
    case KUNAI::DEX::dexinsttype_t::DEX_INSTRUCTION22C:
        gen_instruction(reinterpret_cast<KUNAI::DEX::Instruction22c *>(instr));
        break;
    default:
        throw exceptions::LifterException("MjolnIRLifter::gen_instruction: InstructionType not implemented");
    }
}

void Lifter::gen_instruction(KUNAI::DEX::Instruction23x *instr)
{
    auto op_code = instr->get_instruction_opcode();

    auto location = mlir::FileLineColLoc::get(&context, module_name, instr->get_address(), 0);

    auto dest = instr->get_destination();
    auto src1 = instr->get_first_source();
    auto src2 = instr->get_second_source();

    mlir::Type dest_type = nullptr;

    switch (op_code)
    {
    /// Different Add Operations
    case KUNAI::DEX::TYPES::OP_ADD_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_ADD_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_ADD_FLOAT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_ADD_DOUBLE:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::AddOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;
    /// Different Sub operations
    case KUNAI::DEX::TYPES::OP_SUB_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_SUB_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_SUB_FLOAT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_SUB_DOUBLE:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::SubOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;
    /// Different Mul operations
    case KUNAI::DEX::TYPES::OP_MUL_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_MUL_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_MUL_FLOAT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_MUL_DOUBLE:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::MulOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;
    /// Different Div operations
    case KUNAI::DEX::TYPES::OP_DIV_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_DIV_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_DIV_FLOAT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_DIV_DOUBLE:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::DivOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;
    /// Different Rem operations
    case KUNAI::DEX::TYPES::OP_REM_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_REM_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_REM_FLOAT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_REM_DOUBLE:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::RemOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    /// All And operations
    case KUNAI::DEX::TYPES::OP_AND_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_AND_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::AndOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    /// All Or operations
    case KUNAI::DEX::TYPES::OP_OR_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_OR_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::OrOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    /// All Xor operations
    case KUNAI::DEX::TYPES::OP_XOR_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_XOR_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::XorOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    /// All SHL instructions
    case KUNAI::DEX::TYPES::OP_SHL_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_SHL_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::Shl>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    /// All SHR instructions
    case KUNAI::DEX::TYPES::OP_SHR_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_SHR_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::Shr>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    /// All USHR instructions
    case KUNAI::DEX::TYPES::OP_USHR_INT:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_USHR_LONG:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src1);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src2);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::UShr>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    default:
        throw exceptions::LifterException("MjolnIRLifter::gen_instruction: Opcode from Instruction23x not implemented");
        break;
    }
}

void Lifter::gen_instruction(KUNAI::DEX::Instruction11x *instr)
{
    auto op_code = instr->get_instruction_opcode();

    auto location = mlir::FileLineColLoc::get(&context, module_name, instr->get_address(), 0);

    auto dest = instr->get_destination();

    switch (op_code)
    {
    case KUNAI::DEX::TYPES::OP_RETURN:
    case KUNAI::DEX::TYPES::OP_RETURN_WIDE:
    case KUNAI::DEX::TYPES::OP_RETURN_OBJECT:
    {
        auto reg_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);

        builder.create<::mlir::KUNAI::MjolnIR::ReturnOp>(
            location,
            reg_value);
    }
    break;
    default:
        throw exceptions::LifterException("MjolnIRLifter::gen_instruction: Instruction11x not supported");
        break;
    }
}

void Lifter::gen_instruction(KUNAI::DEX::Instruction12x *instr)
{
    auto op_code = instr->get_instruction_opcode();

    auto location = mlir::FileLineColLoc::get(&context, module_name, instr->get_address(), 0);

    auto dest = instr->get_destination();
    auto src = instr->get_source();

    mlir::Type dest_type = nullptr;

    switch (op_code)
    {
    case KUNAI::DEX::TYPES::OP_ADD_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_ADD_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_ADD_FLOAT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_ADD_DOUBLE_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::AddOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_SUB_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_SUB_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_SUB_FLOAT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_SUB_DOUBLE_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::SubOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_MUL_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_MUL_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_MUL_FLOAT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_MUL_DOUBLE_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::MulOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_DIV_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_DIV_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_DIV_FLOAT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_DIV_DOUBLE_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::DivOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_REM_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_REM_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
    case KUNAI::DEX::TYPES::OP_REM_FLOAT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMFloatType::get(&context);
    case KUNAI::DEX::TYPES::OP_REM_DOUBLE_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMDoubleType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::RemOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_AND_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_AND_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::AndOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_OR_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_OR_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::OrOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_XOR_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_XOR_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::XorOp>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_SHL_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_SHL_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::Shl>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_SHR_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_SHR_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::Shr>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    case KUNAI::DEX::TYPES::OP_USHR_INT_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMIntType::get(&context);
    case KUNAI::DEX::TYPES::OP_USHR_LONG_2ADDR:
        if (!dest_type)
            dest_type = ::mlir::KUNAI::MjolnIR::DVMLongType::get(&context);
        {
            auto src1_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), dest);
            auto src2_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), src);

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::UShr>(
                location,
                dest_type,
                src1_value,
                src2_value);

            writeLocalVariable(current_basic_block, dest, generated_value);
        }
        break;

    default:
        throw exceptions::LifterException("MjolnIRLifter::gen_instruction: Instruction12x not supported");
    }
}

void Lifter::gen_instruction(KUNAI::DEX::Instruction22c *instr)
{
    auto op_code = instr->get_instruction_opcode();

    auto location = mlir::FileLineColLoc::get(&context, module_name, instr->get_address(), 0);

    auto reg = instr->get_destination();

    mlir::Type destination_type;

    switch (op_code)
    {
    case KUNAI::DEX::TYPES::OP_IGET_BOOLEAN:
        if (!destination_type)
            destination_type = ::mlir::KUNAI::MjolnIR::DVMBoolType::get(&context);
    case KUNAI::DEX::TYPES::OP_IGET_BYTE:
        if (!destination_type)
            destination_type = ::mlir::KUNAI::MjolnIR::DVMByteType::get(&context);
    case KUNAI::DEX::TYPES::OP_IGET_CHAR:
        if (!destination_type)
            destination_type = ::mlir::KUNAI::MjolnIR::DVMCharType::get(&context);
    case KUNAI::DEX::TYPES::OP_IGET_SHORT:
        if (!destination_type)
            destination_type = ::mlir::KUNAI::MjolnIR::DVMShortType::get(&context);
        {
            auto field = instr->get_checked_field();
            auto field_ref = instr->get_checked_id();

            std::string &field_name = field->pretty_field();

            auto generated_value = builder.create<::mlir::KUNAI::MjolnIR::LoadFieldOp>(
                location,
                destination_type,
                field_name,
                field_ref);

            writeLocalVariable(current_basic_block, reg, generated_value);
        }
        break;
    case KUNAI::DEX::TYPES::OP_IPUT_BOOLEAN:
    case KUNAI::DEX::TYPES::OP_IPUT_BYTE:
    case KUNAI::DEX::TYPES::OP_IPUT_CHAR:
    case KUNAI::DEX::TYPES::OP_IPUT_SHORT:
    {
        auto field = instr->get_checked_field();
        auto field_ref = instr->get_checked_id();

        std::string &field_name = field->pretty_field();

        auto reg_value = readLocalVariable(current_basic_block, current_method->get_basic_blocks(), reg);

        builder.create<::mlir::KUNAI::MjolnIR::StoreFieldOp>(
            location,
            reg_value,
            field_name,
            field_ref);
    }
    break;
    default:
        throw exceptions::LifterException("MjolnIRLifter::gen_instruction: Instruction22c not implemented yet");
        break;
    }
}
