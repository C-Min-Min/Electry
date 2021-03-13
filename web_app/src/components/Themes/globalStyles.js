import { createGlobalStyle} from "styled-components"
export const GlobalStyles = createGlobalStyle`
  body {
    background: ${({ theme }) => theme.body};
    color: ${({ theme }) => theme.text};  }
  a{
    color: ${({ theme }) => theme.text}; 
  }
  .box{
    background: ${({ theme }) => theme.box};
  }
  
`