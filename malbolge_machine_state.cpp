/**
 * @file malbolge_machine_state.cpp
 * @see malbolge_machine_state.hpp
 */

#include "malbolge_machine_state.hpp"

/**
 * @copydoc malbolge_machine_state::check_memory(const malbolge::word)
 */
std::optional<malbolge::word> malbolge_machine_state::check_memory(const malbolge::word address) const
{
    return memory_diffs.contains(address) ? memory_diffs.at(address)
         : parent ? parent->check_memory(address)
         : std::nullopt;
}

/**
 * @copydoc malbolge_machine_state::access_memory(const malbolge::word)
 */
malbolge::word malbolge_machine_state::access_memory(const malbolge::word address) const
{
    if (const auto mem = check_memory(address)) {
        return *mem;
    } else {
        throw memory_uninitialized_exception(address);
    }
}

/**
 * @copydoc malbolge_machine_state::operate()
 */
malbolge_machine_state::ExecutionStatus malbolge_machine_state::operate()
{
    const auto opcode = malbolge::decode_operation(C, access_memory(C));
    if (!opcode) {
        return ExecutionStatus::Aborted;
    }
    if (*opcode == malbolge::Instruction::MovD) {
        D = access_memory(D);
    } else if (*opcode == malbolge::Instruction::Jmp) {
        C = access_memory(D);
    } else if (*opcode == malbolge::Instruction::RotR) {
        A = memory_diffs[D] = malbolge::trit_rotate_right(access_memory(D));
    } else if (*opcode == malbolge::Instruction::Op) {
        A = memory_diffs[D] = malbolge::op(A, access_memory(D));
    } else if (*opcode == malbolge::Instruction::Out) {
        output += static_cast<unsigned char>(A);
    } else if (*opcode == malbolge::Instruction::In) {
        return ExecutionStatus::Aborted;
    } else if (*opcode == malbolge::Instruction::Exit) {
        return ExecutionStatus::Exited;
    } else /* *opcode == malbolge::Instruction::Nop */ {
        ;
    }
    next_process = &malbolge_machine_state::increment;
    return ExecutionStatus::Running;
}

/**
 * @copydoc malbolge_machine_state::increment()
 */
malbolge_machine_state::ExecutionStatus malbolge_machine_state::increment()
{
    const auto encrypted = malbolge::encrypt_code(access_memory(C));
    if (!encrypted) {
        return ExecutionStatus::Aborted;
    }
    memory_diffs[C] = *encrypted;
    C = (C + 1) % malbolge::word_size;
    D = (D + 1) % malbolge::word_size;
    next_process = &malbolge_machine_state::operate;
    return ExecutionStatus::Running;
}

/**
 * @copydoc malbolge_machine_state::get_code()
 */
std::string malbolge_machine_state::get_code()
{
    std::map<malbolge::word, malbolge::word> original_memory;
    for (auto sp = shared_from_this(); sp->parent; sp = sp->parent) {
        original_memory.insert(sp->written_word);
    }
    std::string code;
    for (malbolge::word i = 0; i <= original_memory.rbegin()->first; ++i) {
        code += original_memory.contains(i) ? original_memory[i] : malbolge::encode_instruction(i, malbolge::Instruction::Nop);
    }
    return code;
}
