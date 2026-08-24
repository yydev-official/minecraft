#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

class shader_program {
    public:
        unsigned int id = 0;

        void load_from_source(const std::string& vs_code, const std::string& fs_code) {
            const char* v_shader_code = vs_code.c_str();
            const char* f_shader_code = fs_code.c_str();

            unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &v_shader_code, NULL);
            glCompileShader(vs);
            check_compile_errors(vs, "VERTEX");

            unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fs, 1, &f_shader_code, NULL);
            glCompileShader(fs);
            check_compile_errors(fs, "FRAGMENT");

            id = glCreateProgram();
            glAttachShader(id, vs);
            glAttachShader(id, fs);
            glLinkProgram(id);
            check_compile_errors(id, "PROGRAM");

            glDeleteShader(vs);
            glDeleteShader(fs);
        }

        void use() const {
            glUseProgram(id);
        }

        void set_mat4(const std::string& name, const glm::mat4& mat) const {
            glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void set_vec3(const std::string& name, const glm::vec3& value) const {
            glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
        }

        void set_int(const std::string& name, int value) const {
            glUniform1i(glGetUniformLocation(id, name.c_str()), value);
        }

        void set_float(const std::string& name, int value) const {
            glUniform1f(glGetUniformLocation(id, name.c_str()), value);
        }

    private:
        void check_compile_errors(unsigned int shader, std::string type) {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n";
                }
            }
        }
};